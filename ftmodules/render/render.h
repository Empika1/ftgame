#ifndef FTRENDER_H
#define FTRENDER_H

#include "ftsim.h"
#include "core/math/color.h"
#include "core/object/ref_counted.h"
#include "core/variant/variant.h"
#include "scene/2d/multimesh_instance_2d.h"
#include "scene/main/node.h"
#include "scene/resources/3d/primitive_meshes.h"
#include "scene/resources/image_texture.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"

struct FTObjType {
    enum Type : uint8_t {
        STATIC_RECT_BORDER,
        STATIC_RECT_INSIDE,
        STATIC_CIRC_BORDER,
        STATIC_CIRC_INSIDE,
        DYNAMIC_RECT_BORDER,
        DYNAMIC_RECT_INSIDE,
        DYNAMIC_CIRC_BORDER,
        DYNAMIC_CIRC_INSIDE,
        GP_RECT_BORDER,
        GP_RECT_INSIDE,
        GP_CIRC_BORDER,
        GP_CIRC_INSIDE,
        UPW_BORDER,
        UPW_INSIDE,
        UPW_DECAL,
        CW_BORDER,
        CW_INSIDE,
        CW_DECAL,
        CCW_BORDER,
        CCW_INSIDE,
        CCW_DECAL,
        WATER_BORDER,
        WATER_INSIDE,
        WOOD_BORDER,
        WOOD_INSIDE,
        BUILD_BORDER,
        BUILD_INSIDE,
        GOAL_BORDER,
        GOAL_INSIDE,
        JOINT_NORMAL,
        JOINT_WHEEL_CENTER,
        SIZE,
    };
};

struct FTRenderPieceType {
    enum Type : uint8_t {
        STATIC_RECT = ft_piece_type::STATIC_RECT,
        STATIC_CIRC = ft_piece_type::STATIC_CIRC,
        DYNAMIC_RECT = ft_piece_type::DYNAMIC_RECT,
        DYNAMIC_CIRC = ft_piece_type::DYNAMIC_CIRC,
        GP_RECT = ft_piece_type::GP_RECT,
        GP_CIRC = ft_piece_type::GP_CIRC,
        UPW = ft_piece_type::UPW,
        CW = ft_piece_type::CW,
        CCW = ft_piece_type::CCW,
        WATER = ft_piece_type::WATER,
        WOOD = ft_piece_type::WOOD,
        BUILD = ft_piece_type::SIZE, //ensure it's larger than any previous
        GOAL,
        SIZE
    };
};

struct FTSdfType {
    enum Type : uint8_t { ROUNDED_RECT, UPW, CW, CCW, SIZE };
};

static const int LAYER_COUNT = 3;

struct RenderLayer {
    MultiMeshInstance2D* mmi;
    PackedVector2Array sizes;
    PackedFloat32Array rotations;
    PackedVector2Array poses;
    Vector<FTObjType::Type> objTypes;
    uint32_t layerID;

    int32_t renderCount = 0;
    void addRenderObject(Vector2 pos, Vector2 size, float rotation, FTObjType::Type type,
                         int32_t multimeshInstanceCount);

    void resetRender();

    void renderPartial(float scale, Vector2 shift, float aaWidth, PackedColorArray colors,
                       PackedFloat32Array cornerRadii, PackedFloat32Array borderThicknesses,
                       bool (*getObjIsCircle)(FTObjType::Type),
                       FTSdfType::Type (*getObjSdfType)(FTObjType::Type), Ref<Image>& renderImg,
                       Vector2i dataImageSize) const;

    void init(MultiMeshInstance2D* mmi_, uint32_t layerID_, int32_t multimeshInstanceCount);
};

class FTRender : public Node {
    GDCLASS(FTRender, Node);

  protected:
    static void _bind_methods();

  private:
    int32_t layerMultimeshInstanceCount;
    Vector2i layerDataImageSize;

    PackedColorArray colors;
    PackedFloat32Array cornerRadii;
    PackedFloat32Array borderThicknesses;

    float aaWidth;
    float jointDiameter;
    float innerJointThresholdDiameter;
    Vector2 woodSizePadding;
    Vector2 waterSizePadding;
    float ghostRodPadding;

  public:
    void setColors(const PackedColorArray colors_);
    PackedColorArray getColors() const;

    void setColor(FTObjType::Type objType, Color color);
    Color getColor(FTObjType::Type objType) const;

    void setCornerRadii(const PackedFloat32Array cornerRadii_);
    PackedFloat32Array getCornerRadii() const;

    void setCornerRadius(FTObjType::Type objType, double cornerRadius);
    double getCornerRadius(FTObjType::Type objType) const;

    void setBorderThicknesses(const PackedFloat32Array borderThicknesses_);
    PackedFloat32Array getBorderThicknesses() const;

    void setBorderThickness(FTObjType::Type objType, double borderThickness);
    double getBorderThickness(FTObjType::Type objType) const;

    static FTObjType::Type getPieceBorder(FTRenderPieceType::Type piece);
    static FTObjType::Type getPieceInside(FTRenderPieceType::Type piece);
    static FTObjType::Type getPieceDecal(FTRenderPieceType::Type piece);
    static bool getObjIsCircle(FTObjType::Type obj);
    static FTSdfType::Type getObjSdfType(FTObjType::Type obj);

  private:
    Ref<ShaderMaterial> shaderMaterial;
    RenderLayer layers[LAYER_COUNT]; // 0: areas, 1: borders, 2: insides
    Ref<Image> renderImg;
    Ref<ImageTexture> renderTex;
    void setupRenderData();

  public:
    void resetRender();

    void render(float scale, Vector2 shift);

  private:
    void addRoundedRect(Vector2 pos, Vector2 size, float rotation, FTRenderPieceType::Type type,
                        RenderLayer& borderLayer, RenderLayer& insideLayer);
    void addRoundedRectPiece(Vector2 pos, Vector2 size, float rotation, FTRenderPieceType::Type type);
    void addArea(Vector2 pos, Vector2 size, float rotation, FTRenderPieceType::Type type);
    void addCirclePiece(Vector2 pos, float diameter, float rotation, FTRenderPieceType::Type type);
    void addJoint(Vector2 pos, float rotation, FTObjType::Type type);
    void addRectJoints(Vector2 pos, Vector2 size, float rotation);
    void addJointedRect(Vector2 pos, Vector2 size, float rotation, FTRenderPieceType::Type type);
    void addRodJoints(Vector2 pos, Vector2 size, float rotation);
    void addJointedRod(Vector2 pos, Vector2 size, float rotation, FTRenderPieceType::Type type);
    void addCircleJoints(Vector2 pos, float diameter, float rotation, FTRenderPieceType::Type type);
    void addJointedCircle(Vector2 pos, float diameter, float rotation, FTRenderPieceType::Type type);
    void addDecalCircle(Vector2 pos, float diameter, float rotation, FTRenderPieceType::Type type);

  public:
    void addStaticRect(Vector2 pos, Vector2 size, float rotation);
    void addStaticCirc(Vector2 pos, float diameter, float rotation);
    void addDynamicRect(Vector2 pos, Vector2 size, float rotation);
    void addDynamicCirc(Vector2 pos, float diameter, float rotation);
    void addGPRect(Vector2 pos, Vector2 size, float rotation);
    void addGPCirc(Vector2 pos, float diameter, float rotation);
    void addWood(Vector2 pos, Vector2 size, float rotation);
    void addWater(Vector2 pos, Vector2 size, float rotation);
    void addCW(Vector2 pos, float diameter, float rotation);
    void addCCW(Vector2 pos, float diameter, float rotation);
    void addUPW(Vector2 pos, float diameter, float rotation);
    void addBuildArea(Vector2 pos, Vector2 size, float rotation);
    void addGoalArea(Vector2 pos, Vector2 size, float rotation);
    void addPiece(FTRenderPieceType::Type type, Vector2 pos, Vector2 size,
                  float rotation); // if circle, pos.x is used as diameter

    void initLayers(int32_t layerMultimeshInstanceCount_, Vector2i layerDataImageSize);
    void initResources(Ref<ShaderMaterial> shaderMaterial_, MultiMeshInstance2D* mmiAreas,
                       MultiMeshInstance2D* mmiBorders, MultiMeshInstance2D* mmiInsides);
    void initVisuals(PackedColorArray colors_, PackedFloat32Array cornerRadii_,
                     PackedFloat32Array borderThicknesses, float aaWidth, float jointDiameter_,
                     float innerJointThresholdDiameter_, Vector2 woodSizePadding,
                     Vector2 waterSizePadding, float ghostRodPadding);
};

VARIANT_ENUM_CAST(FTObjType::Type);
VARIANT_ENUM_CAST(FTRenderPieceType::Type);
VARIANT_ENUM_CAST(FTSdfType::Type);

#endif // FTRENDER_H
