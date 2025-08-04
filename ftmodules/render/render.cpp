#include "render.h"
#include "bind_enum_class_constant.h"
#include "core/math/vector2.h"
#include "core/os/memory.h"
#include "core/string/print_string.h"
#include "scene/2d/sprite_2d.h"
#include "scene/resources/image_texture.h"
#include <cmath>
#include <cstdint>

float uintBitsToFloat(uint32_t u) {
    union Pun {
        uint32_t u;
        float f;
    };
    Pun p;
    p.u = u;
    return p.f;
}

// data layout:
// color: 40 bits as fixed point (10 bits per channel)
// size: 48 bits as fixed point (24 per coord)
// corner radius: 19 bits as fixed point
// border thickness: 19 bits as fixed point
// SDF used: 2 bits
// size, radius, and thickness all stored to the precision of 1/8th of a pixel
Color packDataToColor(Color color, Vector2 size, float cornerRadius, float borderThickness,
                      FTSdfType::Type sdfType) {
    uint32_t r = static_cast<uint32_t>(CLAMP(color.r * 1024., 0, 1023));
    uint32_t g = static_cast<uint32_t>(CLAMP(color.g * 1024., 0, 1023));
    uint32_t b = static_cast<uint32_t>(CLAMP(color.b * 1024., 0, 1023));
    uint32_t a = static_cast<uint32_t>(CLAMP(color.a * 1024., 0, 1023));

    uint32_t sizeX = static_cast<uint32_t>(CLAMP(size.x * 8, 0, 0xFFFFFF));
    uint32_t sizeY = static_cast<uint32_t>(CLAMP(size.y * 8, 0, 0xFFFFFF));

    uint32_t radiusBits = static_cast<uint32_t>(CLAMP(cornerRadius * 8, 0, 0x7FFFF));
    uint32_t thicknessBits = static_cast<uint32_t>(CLAMP(borderThickness * 8, 0, 0x7FFFF));

    uint32_t sdfBits = static_cast<uint32_t>(sdfType) & 0x3;

    uint32_t i1 = (r << 22) | (g << 12) | (b << 2) | (a >> 8);
    uint32_t i2 = (a << 24) | (sizeX >> 0);
    uint32_t i3 = (sizeY << 8) | (radiusBits >> 11);
    uint32_t i4 = (radiusBits << 21) | (thicknessBits << 2) | sdfBits;

    return Color{uintBitsToFloat(i1), uintBitsToFloat(i2), uintBitsToFloat(i3),
                 uintBitsToFloat(i4)};
}

void RenderLayer::addRenderObject(Vector2 pos, Vector2 size, float rotation, FTObjType::Type type,
                                  int32_t multimeshInstanceCount) {
    ERR_FAIL_COND_MSG(renderCount >= multimeshInstanceCount, "Too many objects to render!");

    sizes.set(renderCount, size);
    rotations.set(renderCount, rotation);
    poses.set(renderCount, pos);
    objTypes.set(renderCount, type);

    renderCount++;
}

void RenderLayer::resetRender() { renderCount = 0; }

void RenderLayer::renderPartial(float scale, Vector2 shift, float aaWidth, PackedColorArray colors,
                                PackedFloat32Array cornerRadii,
                                PackedFloat32Array borderThicknesses,
                                bool (*getObjIsCircle)(FTObjType::Type),
                                FTSdfType::Type (*getObjSdfType)(FTObjType::Type),
                                Ref<Image>& renderImg, Vector2i dataImageSize) const {
    Ref<MultiMesh> mm = mmi->get_multimesh();
    mmi->set_instance_shader_parameter("aaWidth", aaWidth);
    mm->set_visible_instance_count(renderCount);
    for (int i = 0; i < renderCount; i++) {
        Transform2D transform(rotations[i], (sizes[i] + Vector2{aaWidth, aaWidth}) * scale, 0,
                              poses[i] * scale + shift);
        mm->set_instance_transform_2d(i, transform);

        FTObjType::Type type = objTypes[i];
        Color color = colors[type];
        Vector2 size = sizes[i] * scale;
        float cornerRadius = getObjIsCircle(type) ? size.x * 0.5 : cornerRadii[type] * scale;
        float borderThickness =
            type == FTObjType::JOINT_NORMAL || type == FTObjType::JOINT_WHEEL_CENTER
                ? borderThicknesses[type] * scale
                : INFINITY;
        FTSdfType::Type sdfType = getObjSdfType(type);

        Color data = packDataToColor(color, size, cornerRadius, borderThickness, sdfType);
        renderImg->set_pixel(i % dataImageSize.x + layerID * dataImageSize.x, i / dataImageSize.y,
                             data);
    }
}

void RenderLayer::init(MultiMeshInstance2D* mmi_, uint32_t layerID_,
                       int32_t multimeshInstanceCount) {
    layerID = layerID_;
    mmi = mmi_;
    mmi->set_instance_shader_parameter("layerID", layerID_);

    sizes.resize(multimeshInstanceCount);
    rotations.resize(multimeshInstanceCount);
    poses.resize(multimeshInstanceCount);
    objTypes.resize(multimeshInstanceCount);
}

void FTRender::_bind_methods() {
    BIND_ENUM_CLASS_CONSTANT(FTObjType, STATIC_RECT_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, STATIC_RECT_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, STATIC_CIRC_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, STATIC_CIRC_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, DYNAMIC_RECT_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, DYNAMIC_RECT_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, DYNAMIC_CIRC_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, DYNAMIC_CIRC_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, GP_RECT_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, GP_RECT_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, GP_CIRC_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, GP_CIRC_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, WOOD_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, WOOD_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, WATER_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, WATER_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, CW_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, CW_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, CW_DECAL);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, CCW_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, CCW_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, CCW_DECAL);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, UPW_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, UPW_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, UPW_DECAL);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, BUILD_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, BUILD_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, GOAL_BORDER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, GOAL_INSIDE);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, JOINT_NORMAL);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, JOINT_WHEEL_CENTER);
    BIND_ENUM_CLASS_CONSTANT(FTObjType, SIZE);

    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, STATIC_RECT);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, STATIC_CIRC);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, DYNAMIC_RECT);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, DYNAMIC_CIRC);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, GP_RECT);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, GP_CIRC);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, CW);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, CCW);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, UPW);
	BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, WOOD);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, WATER);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, BUILD);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, GOAL);
    BIND_ENUM_CLASS_CONSTANT(FTRenderPieceType, SIZE);

    BIND_ENUM_CLASS_CONSTANT(FTSdfType, ROUNDED_RECT);
    BIND_ENUM_CLASS_CONSTANT(FTSdfType, CW);
    BIND_ENUM_CLASS_CONSTANT(FTSdfType, CCW);
    BIND_ENUM_CLASS_CONSTANT(FTSdfType, UPW);
    BIND_ENUM_CLASS_CONSTANT(FTSdfType, SIZE);

    BIND_CONSTANT(LAYER_COUNT);

    ClassDB::bind_method(D_METHOD("setColors", "colors"), &FTRender::setColors);
    ClassDB::bind_method(D_METHOD("getColors"), &FTRender::getColors);
    ClassDB::bind_method(D_METHOD("setColor", "objType", "color"), &FTRender::setColor);
    ClassDB::bind_method(D_METHOD("getColor", "objType"), &FTRender::getColor);

    ClassDB::bind_method(D_METHOD("setCornerRadii", "cornerRadii"), &FTRender::setCornerRadii);
    ClassDB::bind_method(D_METHOD("getCornerRadii"), &FTRender::getCornerRadii);
    ClassDB::bind_method(D_METHOD("setCornerRadius", "objType", "cornerRadius"),
                         &FTRender::setCornerRadius);
    ClassDB::bind_method(D_METHOD("getCornerRadius", "objType"), &FTRender::getCornerRadius);

    ClassDB::bind_method(D_METHOD("setBorderThicknesses", "borderThicknesses"),
                         &FTRender::setBorderThicknesses);
    ClassDB::bind_method(D_METHOD("getBorderThicknesses"), &FTRender::getBorderThicknesses);
    ClassDB::bind_method(D_METHOD("setBorderThickness", "objType", "borderThickness"),
                         &FTRender::setBorderThickness);
    ClassDB::bind_method(D_METHOD("getBorderThickness", "objType"), &FTRender::getBorderThickness);

    ADD_PROPERTY(PropertyInfo(Variant::PACKED_COLOR_ARRAY, "colors"), "setColors", "getColors");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_FLOAT64_ARRAY, "cornerRadii"), "setCornerRadii",
                 "getCornerRadii");
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_FLOAT64_ARRAY, "borderThicknesses"),
                 "setBorderThicknesses", "getBorderThicknesses");

    ClassDB::bind_method(D_METHOD("resetRender"), &FTRender::resetRender);
    ClassDB::bind_method(D_METHOD("render", "scale", "shift"), &FTRender::render);

    ClassDB::bind_method(D_METHOD("addStaticRect", "pos", "size", "rotation"),
                         &FTRender::addStaticRect);
    ClassDB::bind_method(D_METHOD("addStaticCirc", "pos", "diameter", "rotation"),
                         &FTRender::addStaticCirc);
    ClassDB::bind_method(D_METHOD("addDynamicRect", "pos", "size", "rotation"),
                         &FTRender::addDynamicRect);
    ClassDB::bind_method(D_METHOD("addDynamicCirc", "pos", "diameter", "rotation"),
                         &FTRender::addDynamicCirc);
    ClassDB::bind_method(D_METHOD("addGPRect", "pos", "size", "rotation"), &FTRender::addGPRect);
    ClassDB::bind_method(D_METHOD("addGPCirc", "pos", "diameter", "rotation"),
                         &FTRender::addGPCirc);
    ClassDB::bind_method(D_METHOD("addWood", "pos", "size", "rotation"), &FTRender::addWood);
    ClassDB::bind_method(D_METHOD("addWater", "pos", "size", "rotation"), &FTRender::addWater);
    ClassDB::bind_method(D_METHOD("addCW", "pos", "diameter", "rotation"), &FTRender::addCW);
    ClassDB::bind_method(D_METHOD("addCCW", "pos", "diameter", "rotation"), &FTRender::addCCW);
    ClassDB::bind_method(D_METHOD("addUPW", "pos", "diameter", "rotation"), &FTRender::addUPW);
    ClassDB::bind_method(D_METHOD("addBuildArea", "pos", "size", "rotation"),
                         &FTRender::addBuildArea);
    ClassDB::bind_method(D_METHOD("addGoalArea", "pos", "size", "rotation"),
                         &FTRender::addGoalArea);
    ClassDB::bind_method(D_METHOD("addPiece", "type", "pos", "size", "rotation"),
                         &FTRender::addPiece);

    ClassDB::bind_method(
        D_METHOD("initLayers", "layerMultimeshInstanceCount", "layerDataImageSize"),
        &FTRender::initLayers);
    ClassDB::bind_method(
        D_METHOD("initResources", "shaderMaterial", "mmiAreas", "mmiBorders", "mmiInsides"),
        &FTRender::initResources);
    ClassDB::bind_method(D_METHOD("initVisuals", "colors", "cornerRadii", "borderThicknesses",
                                  "aaWidth", "jointDiameter", "innerJointThresholdDiameter",
                                  "woodSizePadding", "waterSizePadding", "ghostRodPadding"),
                         &FTRender::initVisuals);
}

void FTRender::setColors(PackedColorArray colors_) { colors = colors_; }

PackedColorArray FTRender::getColors() const { return colors; }

void FTRender::setColor(FTObjType::Type objType, Color color) { colors.set(objType, color); }

Color FTRender::getColor(FTObjType::Type objType) const { return colors[objType]; }

void FTRender::setCornerRadii(PackedFloat32Array cornerRadii_) { cornerRadii = cornerRadii_; }

PackedFloat32Array FTRender::getCornerRadii() const { return cornerRadii; }

void FTRender::setCornerRadius(FTObjType::Type objType, double cornerRadius) {
    cornerRadii.set(objType, cornerRadius);
}

double FTRender::getCornerRadius(FTObjType::Type objType) const { return cornerRadii[objType]; }

void FTRender::setBorderThicknesses(PackedFloat32Array borderThicknesses_) {
    borderThicknesses = borderThicknesses_;
}

PackedFloat32Array FTRender::getBorderThicknesses() const { return borderThicknesses; }

void FTRender::setBorderThickness(FTObjType::Type objType, double borderThickness) {
    borderThicknesses.set(objType, borderThickness);
}

double FTRender::getBorderThickness(FTObjType::Type objType) const {
    return borderThicknesses[objType];
}

FTObjType::Type FTRender::getPieceBorder(FTRenderPieceType::Type piece) {
    switch (piece) {
    case FTRenderPieceType::STATIC_RECT:
        return FTObjType::STATIC_RECT_BORDER;
    case FTRenderPieceType::STATIC_CIRC:
        return FTObjType::STATIC_CIRC_BORDER;
    case FTRenderPieceType::DYNAMIC_RECT:
        return FTObjType::DYNAMIC_RECT_BORDER;
    case FTRenderPieceType::DYNAMIC_CIRC:
        return FTObjType::DYNAMIC_CIRC_BORDER;
    case FTRenderPieceType::GP_RECT:
        return FTObjType::GP_RECT_BORDER;
    case FTRenderPieceType::GP_CIRC:
        return FTObjType::GP_CIRC_BORDER;
    case FTRenderPieceType::UPW:
        return FTObjType::UPW_BORDER;
    case FTRenderPieceType::CW:
        return FTObjType::CW_BORDER;
    case FTRenderPieceType::CCW:
        return FTObjType::CCW_BORDER;
    case FTRenderPieceType::WATER:
        return FTObjType::WATER_BORDER;
    case FTRenderPieceType::WOOD:
        return FTObjType::WOOD_BORDER;
    case FTRenderPieceType::BUILD:
        return FTObjType::BUILD_BORDER;
    case FTRenderPieceType::GOAL:
        return FTObjType::GOAL_BORDER;
    default:
        ERR_FAIL_V_MSG(FTObjType::SIZE, "Invalid piece type!");
    }
}

FTObjType::Type FTRender::getPieceInside(FTRenderPieceType::Type piece) {
    switch (piece) {
    case FTRenderPieceType::STATIC_RECT:
        return FTObjType::STATIC_RECT_INSIDE;
    case FTRenderPieceType::STATIC_CIRC:
        return FTObjType::STATIC_CIRC_INSIDE;
    case FTRenderPieceType::DYNAMIC_RECT:
        return FTObjType::DYNAMIC_RECT_INSIDE;
    case FTRenderPieceType::DYNAMIC_CIRC:
        return FTObjType::DYNAMIC_CIRC_INSIDE;
    case FTRenderPieceType::GP_RECT:
        return FTObjType::GP_RECT_INSIDE;
    case FTRenderPieceType::GP_CIRC:
        return FTObjType::GP_CIRC_INSIDE;
    case FTRenderPieceType::UPW:
        return FTObjType::UPW_INSIDE;
    case FTRenderPieceType::CW:
        return FTObjType::CW_INSIDE;
    case FTRenderPieceType::CCW:
        return FTObjType::CCW_INSIDE;
    case FTRenderPieceType::WATER:
        return FTObjType::WATER_INSIDE;
    case FTRenderPieceType::WOOD:
        return FTObjType::WOOD_INSIDE;
    case FTRenderPieceType::BUILD:
        return FTObjType::BUILD_INSIDE;
    case FTRenderPieceType::GOAL:
        return FTObjType::GOAL_INSIDE;
    default:
        ERR_FAIL_V_MSG(FTObjType::SIZE, "Invalid piece type!");
    }
}

FTObjType::Type FTRender::getPieceDecal(FTRenderPieceType::Type piece) {
    switch (piece) {
    case FTRenderPieceType::UPW:
        return FTObjType::UPW_DECAL;
    case FTRenderPieceType::CW:
        return FTObjType::CW_DECAL;
    case FTRenderPieceType::CCW:
        return FTObjType::CCW_DECAL;
    default:
        ERR_FAIL_V_MSG(FTObjType::SIZE, "This piece type doesn't have a decal!");
    }
}

bool FTRender::getObjIsCircle(FTObjType::Type obj) {
    switch (obj) {
    case FTObjType::STATIC_CIRC_BORDER:
    case FTObjType::STATIC_CIRC_INSIDE:
    case FTObjType::DYNAMIC_CIRC_BORDER:
    case FTObjType::DYNAMIC_CIRC_INSIDE:
    case FTObjType::GP_CIRC_BORDER:
    case FTObjType::GP_CIRC_INSIDE:
    case FTObjType::UPW_BORDER:
    case FTObjType::UPW_INSIDE:
    case FTObjType::UPW_DECAL:
    case FTObjType::CW_BORDER:
    case FTObjType::CW_INSIDE:
    case FTObjType::CW_DECAL:
    case FTObjType::CCW_BORDER:
    case FTObjType::CCW_INSIDE:
    case FTObjType::CCW_DECAL:
    case FTObjType::JOINT_NORMAL:
    case FTObjType::JOINT_WHEEL_CENTER:
        return true;
    default:
        return false;
    }
}

FTSdfType::Type FTRender::getObjSdfType(FTObjType::Type obj) {
    switch (obj) {
    case FTObjType::UPW_DECAL:
        return FTSdfType::UPW;
    case FTObjType::CW_DECAL:
        return FTSdfType::CW;
    case FTObjType::CCW_DECAL:
        return FTSdfType::CCW;
    default:
        return FTSdfType::ROUNDED_RECT;
    }
}

void FTRender::setupRenderData() {
    renderImg = Image::create_empty(layerDataImageSize.x * LAYER_COUNT, layerDataImageSize.y, false,
                                    Image::FORMAT_RGBAF);
    renderTex = ImageTexture::create_from_image(renderImg);
}

void FTRender::resetRender() {
    for (auto& layer : layers) {
        layer.resetRender();
    }
}

void FTRender::render(float scale, Vector2 shift) {
    for (int i = 0; i < LAYER_COUNT; i++) {
        layers[i].renderPartial(scale, shift, aaWidth, colors, cornerRadii, borderThicknesses,
                                &FTRender::getObjIsCircle, &FTRender::getObjSdfType, renderImg,
                                layerDataImageSize);
    }
    renderTex->update(renderImg);
    shaderMaterial->set_shader_parameter("dataLayerSize", layerDataImageSize);
    shaderMaterial->set_shader_parameter("data", renderTex);
}

float getRealInsideSize(float size, float borderThickness) {
    return abs(size - 2 * borderThickness);
}

float getRealBorderSize(float size, float insideSize, float ghostRodPadding) {
    return MAX(size, insideSize + ghostRodPadding * 2);
}

void FTRender::addRoundedRect(Vector2 pos, Vector2 size, float rotation,
                              FTRenderPieceType::Type type, RenderLayer& borderLayer,
                              RenderLayer& insideLayer) {
    FTObjType::Type borderType = FTRender::getPieceBorder(type);
    FTObjType::Type insideType = FTRender::getPieceInside(type);
    Vector2 borderThickness{borderThicknesses[borderType], borderThicknesses[borderType]};
    Vector2 insideSize{getRealInsideSize(size.x, borderThickness.x),
                       getRealInsideSize(size.y, borderThickness.y)};
    Vector2 borderSize{getRealBorderSize(size.x, insideSize.x, ghostRodPadding),
                       getRealBorderSize(size.y, insideSize.y, ghostRodPadding)};
    borderLayer.addRenderObject(pos, borderSize, rotation, borderType, layerMultimeshInstanceCount);
    insideLayer.addRenderObject(pos, insideSize, rotation, insideType, layerMultimeshInstanceCount);
}

void FTRender::addRoundedRectPiece(Vector2 pos, Vector2 size, float rotation,
                                   FTRenderPieceType::Type type) {
    addRoundedRect(pos, size, rotation, type, layers[1], layers[2]);
}

void FTRender::addArea(Vector2 pos, Vector2 size, float rotation, FTRenderPieceType::Type type) {
    addRoundedRect(pos, size, rotation, type, layers[0], layers[0]);
}

void FTRender::addCirclePiece(Vector2 pos, float diameter, float rotation,
                              FTRenderPieceType::Type type) {
    addRoundedRect(pos, Vector2{diameter, diameter}, rotation, type, layers[1], layers[2]);
}

void FTRender::addJoint(Vector2 pos, float rotation, FTObjType::Type type) {
    static const Vector2 size = Vector2{jointDiameter, jointDiameter};
    layers[2].addRenderObject(pos, size, rotation, type, layerMultimeshInstanceCount);
}

void FTRender::addRectJoints(Vector2 pos, Vector2 size, float rotation) {
    addJoint(pos, 0, FTObjType::JOINT_WHEEL_CENTER);
    addJoint(Vector2(size.x * 0.5, size.y * 0.5).rotated(rotation) + pos, 0,
             FTObjType::JOINT_NORMAL);
    addJoint(Vector2(-size.x * 0.5, size.y * 0.5).rotated(rotation) + pos, 0,
             FTObjType::JOINT_NORMAL);
    addJoint(Vector2(size.x * 0.5, -size.y * 0.5).rotated(rotation) + pos, 0,
             FTObjType::JOINT_NORMAL);
    addJoint(Vector2(-size.x * 0.5, -size.y * 0.5).rotated(rotation) + pos, 0,
             FTObjType::JOINT_NORMAL);
}

void FTRender::addJointedRect(Vector2 pos, Vector2 size, float rotation,
                              FTRenderPieceType::Type type) {
    addRoundedRectPiece(pos, size, rotation, type);
    addRectJoints(pos, size, rotation);
}

void FTRender::addRodJoints(Vector2 pos, Vector2 size, float rotation) {
    addJoint(Vector2(size.x * 0.5, 0).rotated(rotation) + pos, 0, FTObjType::JOINT_NORMAL);
    addJoint(Vector2(-size.x * 0.5, 0).rotated(rotation) + pos, 0, FTObjType::JOINT_NORMAL);
}

void FTRender::addJointedRod(Vector2 pos, Vector2 size, float rotation,
                             FTRenderPieceType::Type type) {
    addRoundedRectPiece(pos, size, rotation, type);
    addRodJoints(pos, size, rotation);
}

void FTRender::addCircleJoints(Vector2 pos, float diameter, float rotation,
                               FTRenderPieceType::Type type) {
    addJoint(pos, 0, FTObjType::JOINT_WHEEL_CENTER);
    float radius = diameter * 0.5;
    addJoint(Vector2(radius, 0).rotated(rotation) + pos, 0, FTObjType::JOINT_NORMAL);
    addJoint(Vector2(-radius, 0).rotated(rotation) + pos, 0, FTObjType::JOINT_NORMAL);
    addJoint(Vector2(0, radius).rotated(rotation) + pos, 0, FTObjType::JOINT_NORMAL);
    addJoint(Vector2(0, -radius).rotated(rotation) + pos, 0, FTObjType::JOINT_NORMAL);
    if (diameter > innerJointThresholdDiameter) {
        addJoint(Vector2(innerJointThresholdDiameter, 0).rotated(rotation) + pos, 0,
                 FTObjType::JOINT_NORMAL);
        addJoint(Vector2(-innerJointThresholdDiameter, 0).rotated(rotation) + pos, 0,
                 FTObjType::JOINT_NORMAL);
        addJoint(Vector2(0, innerJointThresholdDiameter).rotated(rotation) + pos, 0,
                 FTObjType::JOINT_NORMAL);
        addJoint(Vector2(0, -innerJointThresholdDiameter).rotated(rotation) + pos, 0,
                 FTObjType::JOINT_NORMAL);
    }
}

void FTRender::addJointedCircle(Vector2 pos, float diameter, float rotation,
                                FTRenderPieceType::Type type) {
    addCirclePiece(pos, diameter, rotation, type);
    addCircleJoints(pos, diameter, rotation, type);
}

void FTRender::addDecalCircle(Vector2 pos, float diameter, float rotation,
                              FTRenderPieceType::Type type) {
    addCirclePiece(pos, diameter, rotation, type);
    FTObjType::Type borderType = FTRender::getPieceBorder(type);
    FTObjType::Type decalType = FTRender::getPieceDecal(type);
    float borderThickness = borderThicknesses[borderType];
    float insideDiameter = getRealInsideSize(diameter, borderThickness);
    Vector2 insideSize{insideDiameter, insideDiameter};
    layers[2].addRenderObject(pos, insideSize, rotation, decalType, layerMultimeshInstanceCount);
    addCircleJoints(pos, diameter, rotation, type);
}

void FTRender::addStaticRect(Vector2 pos, Vector2 size, float rotation) {
    addRoundedRectPiece(pos, size, rotation, FTRenderPieceType::STATIC_RECT);
}

void FTRender::addStaticCirc(Vector2 pos, float diameter, float rotation) {
    addCirclePiece(pos, diameter, rotation, FTRenderPieceType::STATIC_CIRC);
}

void FTRender::addDynamicRect(Vector2 pos, Vector2 size, float rotation) {
    addRoundedRectPiece(pos, size, rotation, FTRenderPieceType::DYNAMIC_RECT);
}

void FTRender::addDynamicCirc(Vector2 pos, float diameter, float rotation) {
    addCirclePiece(pos, diameter, rotation, FTRenderPieceType::DYNAMIC_CIRC);
}

void FTRender::addGPRect(Vector2 pos, Vector2 size, float rotation) {
    addJointedRect(pos, size, rotation, FTRenderPieceType::GP_RECT);
}

void FTRender::addGPCirc(Vector2 pos, float diameter, float rotation) {
    addJointedCircle(pos, diameter, rotation, FTRenderPieceType::GP_CIRC);
}

void FTRender::addWood(Vector2 pos, Vector2 size, float rotation) {
    addRoundedRectPiece(pos, size + woodSizePadding, rotation, FTRenderPieceType::WOOD);
    addRodJoints(pos, size, rotation);
}

void FTRender::addWater(Vector2 pos, Vector2 size, float rotation) {
    addRoundedRectPiece(pos, size + waterSizePadding, rotation, FTRenderPieceType::WATER);
    addRodJoints(pos, size, rotation);
}

void FTRender::addCW(Vector2 pos, float diameter, float rotation) {
    addDecalCircle(pos, diameter, rotation, FTRenderPieceType::CW);
}

void FTRender::addCCW(Vector2 pos, float diameter, float rotation) {
    addDecalCircle(pos, diameter, rotation, FTRenderPieceType::CCW);
}

void FTRender::addUPW(Vector2 pos, float diameter, float rotation) {
    addDecalCircle(pos, diameter, rotation, FTRenderPieceType::UPW);
}

void FTRender::addBuildArea(Vector2 pos, Vector2 size, float rotation) {
    addArea(pos, size, rotation, FTRenderPieceType::BUILD);
}

void FTRender::addGoalArea(Vector2 pos, Vector2 size, float rotation) {
    addArea(pos, size, rotation, FTRenderPieceType::GOAL);
}

void FTRender::addPiece(FTRenderPieceType::Type type, Vector2 pos, Vector2 size, float rotation) {
    switch (type) {
    case FTRenderPieceType::STATIC_RECT:
        addStaticRect(pos, size, rotation);
        break;
    case FTRenderPieceType::STATIC_CIRC:
        addStaticCirc(pos, size.x, rotation);
        break;
    case FTRenderPieceType::DYNAMIC_RECT:
        addDynamicRect(pos, size, rotation);
        break;
    case FTRenderPieceType::DYNAMIC_CIRC:
        addDynamicCirc(pos, size.x, rotation);
        break;
    case FTRenderPieceType::GP_RECT:
        addGPRect(pos, size, rotation);
        break;
    case FTRenderPieceType::GP_CIRC:
        addGPCirc(pos, size.x, rotation);
        break;
    case FTRenderPieceType::UPW:
        addUPW(pos, size.x, rotation);
        break;
    case FTRenderPieceType::CW:
        addCW(pos, size.x, rotation);
        break;
    case FTRenderPieceType::CCW:
        addCCW(pos, size.x, rotation);
        break;
    case FTRenderPieceType::WATER:
        addWater(pos, size, rotation);
        break;
    case FTRenderPieceType::WOOD:
        addWood(pos, size, rotation);
        break;
    case FTRenderPieceType::BUILD:
        addBuildArea(pos, size, rotation);
        break;
    case FTRenderPieceType::GOAL:
        addBuildArea(pos, size, rotation);
        break;
    default:
        ERR_FAIL_MSG("trying to add invalid piece type");
    }
}

void FTRender::initLayers(int32_t layerMultimeshInstanceCount_, Vector2i layerDataImageSize_) {
    layerMultimeshInstanceCount = layerMultimeshInstanceCount_;
    layerDataImageSize = layerDataImageSize_;
}

void FTRender::initResources(Ref<ShaderMaterial> shaderMaterial_, MultiMeshInstance2D* mmiAreas,
                             MultiMeshInstance2D* mmiBorders, MultiMeshInstance2D* mmiInsides) {

    shaderMaterial = shaderMaterial_;

    layers[0].init(mmiAreas, 0, layerMultimeshInstanceCount);
    layers[1].init(mmiBorders, 1, layerMultimeshInstanceCount);
    layers[2].init(mmiInsides, 2, layerMultimeshInstanceCount);

    setupRenderData();
}

void FTRender::initVisuals(PackedColorArray colors_, PackedFloat32Array cornerRadii_,
                           PackedFloat32Array borderThicknesses_, float aaWidth_,
                           float jointDiameter_, float innerJointThresholdDiameter_,
                           Vector2 woodSizePadding_, Vector2 waterSizePadding_,
                           float ghostRodPadding_) {
    colors = colors_;
    cornerRadii = cornerRadii_;
    borderThicknesses = borderThicknesses_;
    aaWidth = aaWidth_;
    jointDiameter = jointDiameter_;
    innerJointThresholdDiameter = innerJointThresholdDiameter_;
    woodSizePadding = woodSizePadding_;
    waterSizePadding = waterSizePadding_;
    ghostRodPadding = ghostRodPadding_;
}