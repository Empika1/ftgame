#include "sim_adapter.h"

void FTRect::_bind_methods() {
    ClassDB::bind_static_method("FTRect", D_METHOD("init", "x", "y", "w", "h"), &FTRect::init);
    ClassDB::bind_method(D_METHOD("set_x", "x"), &FTRect::set_x);
    ClassDB::bind_method(D_METHOD("get_x"), &FTRect::get_x);
    ClassDB::bind_method(D_METHOD("set_y", "y"), &FTRect::set_y);
    ClassDB::bind_method(D_METHOD("get_y"), &FTRect::get_y);
    ClassDB::bind_method(D_METHOD("set_w", "w"), &FTRect::set_w);
    ClassDB::bind_method(D_METHOD("get_w"), &FTRect::get_w);
    ClassDB::bind_method(D_METHOD("set_h", "h"), &FTRect::set_h);
    ClassDB::bind_method(D_METHOD("get_h"), &FTRect::get_h);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "x"), "set_x", "get_x");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "y"), "set_y", "get_y");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "w"), "set_w", "get_w");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "h"), "set_h", "get_h");
}

Ref<FTRect> FTRect::init(double x, double y, double w, double h) {
    Ref<FTRect> rect;
    rect.instantiate();
    ft_rect rect_{x, y, w, h};
    rect->rect = rect_;
    return rect;
}

void FTRect::set_x(double x) { rect.x = x; }

double FTRect::get_x() const { return rect.x; }

void FTRect::set_y(double y) { rect.y = y; }

double FTRect::get_y() const { return rect.y; }

void FTRect::set_w(double w) { rect.w = w; }

double FTRect::get_w() const { return rect.w; }

void FTRect::set_h(double h) { rect.h = h; }

double FTRect::get_h() const { return rect.h; }

void FTBlockSpec::_bind_methods() {
    ClassDB::bind_static_method(
        "FTBlockSpec", D_METHOD("init", "type", "id", "x", "y", "w", "h", "angle", "j1", "j2"),
        &FTBlockSpec::init);
    ClassDB::bind_method(D_METHOD("set_type", "type"), &FTBlockSpec::set_type);
    ClassDB::bind_method(D_METHOD("get_type"), &FTBlockSpec::get_type);
    ClassDB::bind_method(D_METHOD("set_id", "id"), &FTBlockSpec::set_id);
    ClassDB::bind_method(D_METHOD("get_id"), &FTBlockSpec::get_id);
    ClassDB::bind_method(D_METHOD("set_x", "x"), &FTBlockSpec::set_x);
    ClassDB::bind_method(D_METHOD("get_x"), &FTBlockSpec::get_x);
    ClassDB::bind_method(D_METHOD("set_y", "y"), &FTBlockSpec::set_y);
    ClassDB::bind_method(D_METHOD("get_y"), &FTBlockSpec::get_y);
    ClassDB::bind_method(D_METHOD("set_w", "w"), &FTBlockSpec::set_w);
    ClassDB::bind_method(D_METHOD("get_w"), &FTBlockSpec::get_w);
    ClassDB::bind_method(D_METHOD("set_h", "h"), &FTBlockSpec::set_h);
    ClassDB::bind_method(D_METHOD("get_h"), &FTBlockSpec::get_h);
    ClassDB::bind_method(D_METHOD("set_angle", "angle"), &FTBlockSpec::set_angle);
    ClassDB::bind_method(D_METHOD("get_angle"), &FTBlockSpec::get_angle);
    ClassDB::bind_method(D_METHOD("set_joint_1", "joint"), &FTBlockSpec::set_joint_1);
    ClassDB::bind_method(D_METHOD("get_joint_1"), &FTBlockSpec::get_joint_1);
    ClassDB::bind_method(D_METHOD("set_joint_2", "joint"), &FTBlockSpec::set_joint_2);
    ClassDB::bind_method(D_METHOD("get_joint_2"), &FTBlockSpec::get_joint_2);
    ClassDB::bind_method(D_METHOD("to_block"), &FTBlockSpec::to_block);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "x"), "set_x", "get_x");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "y"), "set_y", "get_y");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "w"), "set_w", "get_w");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "h"), "set_h", "get_h");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "angle"), "set_angle", "get_angle");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "joint_1"), "set_joint_1", "get_joint_1");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "joint_2"), "set_joint_2", "get_joint_2");
}

Ref<FTBlockSpec> FTBlockSpec::init(FTSimPieceType::Type type, uint16_t id, double x, double y,
                                   double w, double h, double angle, uint16_t j1, uint16_t j2) {
    Ref<FTBlockSpec> block;
    block.instantiate();
    ft_block_spec spec{static_cast<ft_piece_type::type>(type), id, x, y, w, h, angle, {j1, j2}};
    block->spec = spec;
    return block;
}

void FTBlockSpec::set_type(FTSimPieceType::Type type) {
    spec.type = static_cast<ft_piece_type::type>(type);
}

FTSimPieceType::Type FTBlockSpec::get_type() const {
    return static_cast<FTSimPieceType::Type>(spec.type);
}

void FTBlockSpec::set_id(uint16_t id) { spec.id = id; }

uint16_t FTBlockSpec::get_id() const { return spec.id; }

void FTBlockSpec::set_x(double x) { spec.x = x; }

double FTBlockSpec::get_x() const { return spec.x; }

void FTBlockSpec::set_y(double y) { spec.y = y; }

double FTBlockSpec::get_y() const { return spec.y; }

void FTBlockSpec::set_w(double w) { spec.w = w; }

double FTBlockSpec::get_w() const { return spec.w; }

void FTBlockSpec::set_h(double h) { spec.h = h; }

double FTBlockSpec::get_h() const { return spec.h; }

void FTBlockSpec::set_angle(double angle) { spec.angle = angle; }

double FTBlockSpec::get_angle() const { return spec.angle; }

void FTBlockSpec::set_joint_1(uint16_t joint) { spec.joints[0] = joint; }

uint16_t FTBlockSpec::get_joint_1() const { return spec.joints[0]; }

void FTBlockSpec::set_joint_2(uint16_t joint) { spec.joints[1] = joint; }

uint16_t FTBlockSpec::get_joint_2() const { return spec.joints[1]; }

Ref<FTBlock> FTBlockSpec::to_block() const {
    Ref<FTBlock> block;
    block.instantiate();
    block->block = ::to_block(spec);
    return block;
}

void FTDesignSpec::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_blocks", "blocks"), &FTDesignSpec::set_blocks);
    ClassDB::bind_method(D_METHOD("get_blocks"), &FTDesignSpec::get_blocks);
    ClassDB::bind_method(D_METHOD("set_blocks_packed", "type", "x", "y", "w", "h", "r", "j1", "j2"),
                         &FTDesignSpec::set_blocks_packed);
    ClassDB::bind_method(D_METHOD("get_slice"), &FTDesignSpec::get_slice);
    ClassDB::bind_method(D_METHOD("set_build", "rect"), &FTDesignSpec::set_build);
    ClassDB::bind_method(D_METHOD("get_build"), &FTDesignSpec::get_build);
    ClassDB::bind_method(D_METHOD("set_goal", "rect"), &FTDesignSpec::set_goal);
    ClassDB::bind_method(D_METHOD("get_goal"), &FTDesignSpec::get_goal);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "blocks"), "set_blocks", "get_blocks");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "build"), "set_build", "get_build");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "goal"), "set_goal", "get_goal");
}

void FTDesignSpec::set_blocks(const TypedArray<FTBlockSpec> blocks) {
    spec.blocks = std::vector<ft_block_spec>();
    for (int i = 0; i < blocks.size(); ++i) {
        Ref<FTBlockSpec> block = blocks[i];
        ft_block_spec bdef = block->spec;
        spec.blocks.push_back(bdef);
    }
}

TypedArray<FTBlockSpec> FTDesignSpec::get_blocks() const {
    TypedArray<FTBlockSpec> result;

    for (size_t j = 0; j < spec.blocks.size(); ++j) {
        const ft_block_spec& ftb = spec.blocks[j];
        Ref<FTBlockSpec> block;
        block.instantiate();
        block->spec = ftb;
        result.push_back(block);
    }

    return result;
}

void FTDesignSpec::set_blocks_packed(const PackedByteArray t, const PackedFloat64Array x,
                                     const PackedFloat64Array y, const PackedFloat64Array w,
                                     const PackedFloat64Array h, const PackedFloat64Array r,
                                     const PackedInt32Array j1, const PackedInt32Array j2) {
    spec.blocks = std::vector<ft_block_spec>();
    int j = 0;
    for (int i = 0; i < t.size(); ++i) {
        ft_block_spec bdef{static_cast<ft_piece_type::type>(t[i]),
                           FT_NO_ID,
                           x[i],
                           y[i],
                           w[i],
                           h[i],
                           r[i],
                           static_cast<uint16_t>(j1[i]),
                           static_cast<uint16_t>(j2[i])};
        if (ft_is_player_movable(bdef.type)) {
            bdef.id = j;
            ++j;
        }
        spec.blocks.push_back(bdef);
    }
}

PackedFloat64Array FTDesignSpec::get_slice(int pi) const {
    PackedFloat64Array result;

    for (size_t j = 0; j < spec.blocks.size(); ++j) {
        const ft_block_spec& ftb = spec.blocks[j];
        double data = 0;
        switch (pi) {
        case 0:
            data = static_cast<double>(ftb.type);
            break;
        case 1:
            data = static_cast<double>(ftb.id);
            break;
        case 2:
            data = ftb.x;
            break;
        case 3:
            data = ftb.y;
            break;
        case 4:
            data = ftb.w;
            break;
        case 5:
            data = ftb.h;
            break;
        case 6:
            data = ftb.angle;
            break;
        }
        result.push_back(data);
    }

    return result;
}

void FTDesignSpec::set_build(const Ref<FTRect> rect) { spec.build = rect->rect; }

Ref<FTRect> FTDesignSpec::get_build() const {
    Ref<FTRect> rect;
    rect.instantiate();
    rect->rect = spec.build;
    return rect;
}

void FTDesignSpec::set_goal(const Ref<FTRect> rect) { spec.goal = rect->rect; }

Ref<FTRect> FTDesignSpec::get_goal() const {
    Ref<FTRect> rect;
    rect.instantiate();
    rect->rect = spec.goal;
    return rect;
}

void FTBlock::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_block_idx", "idx"), &FTBlock::set_block_idx);
    ClassDB::bind_method(D_METHOD("get_block_idx"), &FTBlock::get_block_idx);
    ClassDB::bind_method(D_METHOD("set_type", "type"), &FTBlock::set_type);
    ClassDB::bind_method(D_METHOD("get_type"), &FTBlock::get_type);
    ClassDB::bind_method(D_METHOD("set_x", "x"), &FTBlock::set_x);
    ClassDB::bind_method(D_METHOD("get_x"), &FTBlock::get_x);
    ClassDB::bind_method(D_METHOD("set_y", "y"), &FTBlock::set_y);
    ClassDB::bind_method(D_METHOD("get_y"), &FTBlock::get_y);
    ClassDB::bind_method(D_METHOD("set_w", "w"), &FTBlock::set_w);
    ClassDB::bind_method(D_METHOD("get_w"), &FTBlock::get_w);
    ClassDB::bind_method(D_METHOD("set_h", "h"), &FTBlock::set_h);
    ClassDB::bind_method(D_METHOD("get_h"), &FTBlock::get_h);
    ClassDB::bind_method(D_METHOD("set_angle", "angle"), &FTBlock::set_angle);
    ClassDB::bind_method(D_METHOD("get_angle"), &FTBlock::get_angle);
    ClassDB::bind_method(D_METHOD("set_joint_stack_idx", "js_idx", "i"),
                         &FTBlock::set_joint_stack_idx);
    ClassDB::bind_method(D_METHOD("get_joint_stack_idx", "i"), &FTBlock::get_joint_stack_idx);
    ClassDB::bind_method(D_METHOD("set_joint_stack_idxs", "js_idxs"),
                         &FTBlock::set_joint_stack_idxs);
    ClassDB::bind_method(D_METHOD("get_joint_stack_idxs"), &FTBlock::get_joint_stack_idxs);
    ClassDB::bind_method(D_METHOD("set_joint_idx", "joint_idx", "i"), &FTBlock::set_joint_idx);
    ClassDB::bind_method(D_METHOD("get_joint_idx", "i"), &FTBlock::get_joint_idx);
    ClassDB::bind_method(D_METHOD("set_joint_idxs", "joint_idxs"), &FTBlock::set_joint_idxs);
    ClassDB::bind_method(D_METHOD("get_joint_idxs"), &FTBlock::get_joint_idxs);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "block_idx"), "set_block_idx", "get_block_idx");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "x"), "set_x", "get_x");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "y"), "set_y", "get_y");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "w"), "set_w", "get_w");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "h"), "set_h", "get_h");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "angle"), "set_angle", "get_angle");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "joint_stack_idxs"), "set_joint_stack_idxs",
                 "get_joint_stack_idxs");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "joint_idxs"), "set_joint_idxs", "get_joint_idxs");
}

void FTBlock::set_block_idx(uint16_t idx) { block.block_idx = idx; }

uint16_t FTBlock::get_block_idx() const { return block.block_idx; }

void FTBlock::set_type(FTSimPieceType::Type type) {
    block.type = static_cast<ft_piece_type::type>(type);
}

FTSimPieceType::Type FTBlock::get_type() const {
    return static_cast<FTSimPieceType::Type>(block.type);
}

void FTBlock::set_x(double x) { block.x = x; }

double FTBlock::get_x() const { return block.x; }

void FTBlock::set_y(double y) { block.y = y; }

double FTBlock::get_y() const { return block.y; }

void FTBlock::set_w(double w) { block.w = w; }

double FTBlock::get_w() const { return block.w; }

void FTBlock::set_h(double h) { block.h = h; }

double FTBlock::get_h() const { return block.h; }

void FTBlock::set_angle(double angle) { block.angle = angle; }

double FTBlock::get_angle() const { return block.angle; }

void FTBlock::set_joint_stack_idx(uint16_t js_idx, size_t i) {
    ERR_FAIL_COND_MSG(i >= FT_MAX_JOINTS, "Joint stack index out of range");
    block.joint_stack_idxs[i] = js_idx;
}

uint16_t FTBlock::get_joint_stack_idx(size_t i) {
    ERR_FAIL_COND_V_MSG(i >= FT_MAX_JOINTS, FT_NO_JOINT_STACK, "Joint stack index out of range");
    return block.joint_stack_idxs[i];
}

void FTBlock::set_joint_stack_idxs(const TypedArray<uint16_t> js_idxs) {
    ERR_FAIL_COND_MSG(js_idxs.size() != FT_MAX_JOINTS,
                      "Joint stack index array must be of size FT_MAX_JOINTS");
    for (size_t i = 0; i < FT_MAX_JOINTS; ++i) {
        block.joint_stack_idxs[i] = js_idxs[i];
    }
}

TypedArray<uint16_t> FTBlock::get_joint_stack_idxs() const {
    TypedArray<uint16_t> result;
    for (size_t i = 0; i < FT_MAX_JOINTS; ++i) {
        result.push_back(block.joint_stack_idxs[i]);
    }
    return result;
}

void FTBlock::set_joint_idx(uint16_t joint_idx, size_t i) {
    ERR_FAIL_COND_MSG(i >= FT_MAX_JOINTS, "Joint index out of range");
    block.joint_idxs[i] = joint_idx;
}

uint16_t FTBlock::get_joint_idx(size_t i) {
    ERR_FAIL_COND_V_MSG(i >= FT_MAX_JOINTS, FT_NO_JOINT, "Joint index out of range");
    return block.joint_idxs[i];
}

void FTBlock::set_joint_idxs(const TypedArray<uint16_t> joint_idxs) {
    ERR_FAIL_COND_MSG(joint_idxs.size() != FT_MAX_JOINTS,
                      "Joint index array must be of size FT_MAX_JOINTS");
    for (size_t i = 0; i < FT_MAX_JOINTS; ++i) {
        block.joint_idxs[i] = joint_idxs[i];
    }
}

TypedArray<uint16_t> FTBlock::get_joint_idxs() const {
    TypedArray<uint16_t> result;
    for (size_t i = 0; i < FT_MAX_JOINTS; ++i) {
        result.push_back(block.joint_idxs[i]);
    }
    return result;
}

void FTJoint::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_block_idx", "block_idx"), &FTJoint::set_block_idx);
    ClassDB::bind_method(D_METHOD("get_block_idx"), &FTJoint::get_block_idx);
    ClassDB::bind_method(D_METHOD("set_joint_stack_idx", "joint_stack_idx"),
                         &FTJoint::set_joint_stack_idx);
    ClassDB::bind_method(D_METHOD("get_joint_stack_idx"), &FTJoint::get_joint_stack_idx);
    ClassDB::bind_method(D_METHOD("set_joint_idx", "joint_idx"), &FTJoint::set_joint_idx);
    ClassDB::bind_method(D_METHOD("get_joint_idx"), &FTJoint::get_joint_idx);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "block_idx"), "set_block_idx", "get_block_idx");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "joint_stack_idx"), "set_joint_stack_idx",
                 "get_joint_stack_idx");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "joint_idx"), "set_joint_idx", "get_joint_idx");
}

void FTJoint::set_block_idx(uint16_t block_idx) { joint.block_idx = block_idx; }

uint16_t FTJoint::get_block_idx() const { return joint.block_idx; }

void FTJoint::set_joint_stack_idx(uint16_t joint_stack_idx) {
    joint.joint_stack_idx = joint_stack_idx;
}

uint16_t FTJoint::get_joint_stack_idx() const { return joint.joint_stack_idx; }

void FTJoint::set_joint_idx(uint16_t joint_idx) { joint.joint_idx = joint_idx; }

uint16_t FTJoint::get_joint_idx() const { return joint.joint_idx; }

void FTJointStack::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_joint_stack_idx", "joint_stack_idx"),
                         &FTJointStack::set_joint_stack_idx);
    ClassDB::bind_method(D_METHOD("get_joint_stack_idx"), &FTJointStack::get_joint_stack_idx);
    ClassDB::bind_method(D_METHOD("set_joint", "joint", "i"), &FTJointStack::set_joint);
    ClassDB::bind_method(D_METHOD("get_joint", "i"), &FTJointStack::get_joint);
    ClassDB::bind_method(D_METHOD("set_joints", "joints"), &FTJointStack::set_joints);
    ClassDB::bind_method(D_METHOD("get_joints"), &FTJointStack::get_joints);
    ClassDB::bind_method(D_METHOD("set_x", "x"), &FTJointStack::set_x);
    ClassDB::bind_method(D_METHOD("get_x"), &FTJointStack::get_x);
    ClassDB::bind_method(D_METHOD("set_y", "y"), &FTJointStack::set_y);
    ClassDB::bind_method(D_METHOD("get_y"), &FTJointStack::get_y);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "joint_stack_idx"), "set_joint_stack_idx",
                 "get_joint_stack_idx");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "joints"), "set_joints", "get_joints");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "x"), "set_x", "get_x");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "y"), "set_y", "get_y");
}

void FTJointStack::set_joint_stack_idx(uint16_t joint_stack_idx) {
    joint_stack.joint_stack_idx = joint_stack_idx;
}

uint16_t FTJointStack::get_joint_stack_idx() const { return joint_stack.joint_stack_idx; }

void FTJointStack::set_joint(const Ref<FTJoint> joint, size_t i) {
    ERR_FAIL_COND_MSG(i >= joint_stack.joints.size(), "Joint index out of range");
    joint_stack.joints[i] = joint->joint;
}

Ref<FTJoint> FTJointStack::get_joint(size_t i) const {
    ERR_FAIL_COND_V_MSG(i >= joint_stack.joints.size(), Ref<FTJoint>(), "Joint index out of range");
    Ref<FTJoint> joint;
    joint.instantiate();
    joint->joint = joint_stack.joints[i];
    return joint;
}

void FTJointStack::set_joints(const TypedArray<FTJoint> joints) {
    ERR_FAIL_COND_MSG(static_cast<size_t>(joints.size()) != joint_stack.joints.size(),
                      "Joints array size mismatch");
    for (int i = 0; i < joints.size(); ++i) {
        Ref<FTJoint> joint = joints[i];
        joint_stack.joints[i] = joint->joint;
    }
}

TypedArray<FTJoint> FTJointStack::get_joints() const {
    TypedArray<FTJoint> result;
    for (size_t i = 0; i < joint_stack.joints.size(); ++i) {
        Ref<FTJoint> joint;
        joint.instantiate();
        joint->joint = joint_stack.joints[i];
        result.push_back(joint);
    }
    return result;
}

void FTJointStack::set_x(double x) { joint_stack.x = x; }

double FTJointStack::get_x() const { return joint_stack.x; }

void FTJointStack::set_y(double y) { joint_stack.y = y; }

double FTJointStack::get_y() const { return joint_stack.y; }

void FTDesign::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "spec"), &FTDesign::init);

    ClassDB::bind_method(D_METHOD("set_level_block", "block", "i"), &FTDesign::set_level_block);
    ClassDB::bind_method(D_METHOD("get_level_block", "i"), &FTDesign::get_level_block);
    ClassDB::bind_method(D_METHOD("set_level_blocks", "level_blocks"), &FTDesign::set_level_blocks);
    ClassDB::bind_method(D_METHOD("get_level_blocks"), &FTDesign::get_level_blocks);

    ClassDB::bind_method(D_METHOD("set_design_block", "block", "i"), &FTDesign::set_design_block);
    ClassDB::bind_method(D_METHOD("get_design_block", "i"), &FTDesign::get_design_block);
    ClassDB::bind_method(D_METHOD("set_design_blocks", "design_blocks"),
                         &FTDesign::set_design_blocks);
    ClassDB::bind_method(D_METHOD("get_design_blocks"), &FTDesign::get_design_blocks);

    ClassDB::bind_method(D_METHOD("set_joint_stack", "joint_stack", "i"),
                         &FTDesign::set_joint_stack);
    ClassDB::bind_method(D_METHOD("get_joint_stack", "i"), &FTDesign::get_joint_stack);
    ClassDB::bind_method(D_METHOD("set_joint_stacks", "joint_stacks"), &FTDesign::set_joint_stacks);
    ClassDB::bind_method(D_METHOD("get_joint_stacks"), &FTDesign::get_joint_stacks);

    ClassDB::bind_method(D_METHOD("set_build", "rect"), &FTDesign::set_build);
    ClassDB::bind_method(D_METHOD("get_build"), &FTDesign::get_build);

    ClassDB::bind_method(D_METHOD("set_goal", "rect"), &FTDesign::set_goal);
    ClassDB::bind_method(D_METHOD("get_goal"), &FTDesign::get_goal);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "level_blocks"), "set_level_blocks",
                 "get_level_blocks");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "design_blocks"), "set_design_blocks",
                 "get_design_blocks");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "joint_stacks"), "set_joint_stacks",
                 "get_joint_stacks");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "build"), "set_build", "get_build");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "goal"), "set_goal", "get_goal");
}

void FTDesign::init(const Ref<FTDesignSpec> spec) {
    ERR_FAIL_COND_MSG(spec.is_null(), "Design spec cannot be null");
    design = ft_create_design(design, spec->spec);
}

void FTDesign::set_level_block(const Ref<FTBlock> block, size_t i) {
    ERR_FAIL_COND_MSG(i >= design->level_blocks.size(), "Level block index out of range");
    design->level_blocks[i] = block->block;
}

Ref<FTBlock> FTDesign::get_level_block(size_t i) const {
    ERR_FAIL_COND_V_MSG(i >= design->level_blocks.size(), Ref<FTBlock>(),
                        "Level block index out of range");
    Ref<FTBlock> block;
    block.instantiate();
    block->block = design->level_blocks[i];
    return block;
}

void FTDesign::set_level_blocks(const TypedArray<FTBlock> level_blocks) {
    ERR_FAIL_COND_MSG(static_cast<size_t>(level_blocks.size()) != design->level_blocks.size(),
                      "Level blocks array size mismatch");
    for (int i = 0; i < level_blocks.size(); ++i) {
        Ref<FTBlock> block = level_blocks[i];
        design->level_blocks[i] = block->block;
    }
}

TypedArray<FTBlock> FTDesign::get_level_blocks() const {
    TypedArray<FTBlock> result;
    for (size_t i = 0; i < design->level_blocks.size(); ++i) {
        Ref<FTBlock> block;
        block.instantiate();
        block->block = design->level_blocks[i];
        result.push_back(block);
    }
    return result;
}

void FTDesign::set_design_block(const Ref<FTBlock> block, size_t i) {
    ERR_FAIL_COND_MSG(i >= design->design_blocks.size(), "Design block index out of range");
    design->design_blocks[i] = block->block;
}

Ref<FTBlock> FTDesign::get_design_block(size_t i) const {
    ERR_FAIL_COND_V_MSG(i >= design->design_blocks.size(), Ref<FTBlock>(),
                        "Design block index out of range");
    Ref<FTBlock> block;
    block.instantiate();
    block->block = design->design_blocks[i];
    return block;
}

void FTDesign::set_design_blocks(const TypedArray<FTBlock> design_blocks) {
    ERR_FAIL_COND_MSG(static_cast<size_t>(design_blocks.size()) != design->design_blocks.size(),
                      "Design blocks array size mismatch");
    for (int i = 0; i < design_blocks.size(); ++i) {
        Ref<FTBlock> block = design_blocks[i];
        design->design_blocks[i] = block->block;
    }
}

TypedArray<FTBlock> FTDesign::get_design_blocks() const {
    TypedArray<FTBlock> result;
    for (size_t i = 0; i < design->design_blocks.size(); ++i) {
        Ref<FTBlock> block;
        block.instantiate();
        block->block = design->design_blocks[i];
        result.push_back(block);
    }
    return result;
}

void FTDesign::set_joint_stack(const Ref<FTJointStack> joint_stack, size_t i) {
    ERR_FAIL_COND_MSG(i >= design->joint_stacks.size(), "Joint stack index out of range");
    design->joint_stacks[i] = joint_stack->joint_stack;
}

Ref<FTJointStack> FTDesign::get_joint_stack(size_t i) const {
    ERR_FAIL_COND_V_MSG(i >= design->joint_stacks.size(), Ref<FTJointStack>(),
                        "Joint stack index out of range");
    Ref<FTJointStack> joint_stack;
    joint_stack.instantiate();
    joint_stack->joint_stack = design->joint_stacks[i];
    return joint_stack;
}

void FTDesign::set_joint_stacks(const TypedArray<FTJointStack> joint_stacks) {
    ERR_FAIL_COND_MSG(static_cast<size_t>(joint_stacks.size()) != design->joint_stacks.size(),
                      "Joint stacks array size mismatch");
    for (int i = 0; i < joint_stacks.size(); ++i) {
        Ref<FTJointStack> joint_stack = joint_stacks[i];
        design->joint_stacks[i] = joint_stack->joint_stack;
    }
}

TypedArray<FTJointStack> FTDesign::get_joint_stacks() const {
    TypedArray<FTJointStack> result;
    for (size_t i = 0; i < design->joint_stacks.size(); ++i) {
        Ref<FTJointStack> joint_stack;
        joint_stack.instantiate();
        joint_stack->joint_stack = design->joint_stacks[i];
        result.push_back(joint_stack);
    }
    return result;
}

void FTDesign::set_build(const Ref<FTRect> rect) { design->build = rect->rect; }

Ref<FTRect> FTDesign::get_build() const {
    Ref<FTRect> rect;
    rect.instantiate();
    rect->rect = design->build;
    return rect;
}

void FTDesign::set_goal(const Ref<FTRect> rect) { design->goal = rect->rect; }

Ref<FTRect> FTDesign::get_goal() const {
    Ref<FTRect> rect;
    rect.instantiate();
    rect->rect = design->goal;
    return rect;
}

void FTSimSettings::_bind_methods() {}

void FTSimState::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init", "design", "settings"), &FTSimState::init);
    ClassDB::bind_method(D_METHOD("step", "settings"), &FTSimState::step);
    ClassDB::bind_method(D_METHOD("in_area", "block", "area"), &FTSimState::in_area);
    ClassDB::bind_method(D_METHOD("is_solved", "spec"), &FTSimState::is_solved);
    ClassDB::bind_method(D_METHOD("get_tick"), &FTSimState::get_tick);
    ClassDB::bind_method(D_METHOD("get_level_blocks"), &FTSimState::get_level_blocks);
    ClassDB::bind_method(D_METHOD("get_design_blocks"), &FTSimState::get_design_blocks);
    ClassDB::bind_method(D_METHOD("get_joint_stacks"), &FTSimState::get_joint_stacks);
}

void FTSimState::init(const Ref<FTDesign> design, const Ref<FTSimSettings> settings) {
    ERR_FAIL_COND_MSG(design.is_null(), "Design cannot be null");
    ERR_FAIL_COND_MSG(settings.is_null(), "Sim settings cannot be null");
    state = ft_create_sim(state, *design->design, settings->settings);
}

void FTSimState::step(const Ref<FTSimSettings> settings) {
    ERR_FAIL_COND_MSG(settings.is_null(), "Sim settings cannot be null");
    ft_step_sim(state, settings->settings);
}

bool FTSimState::in_area(const Ref<FTBlock> block, const Ref<FTRect> area) const {
    ERR_FAIL_COND_V_MSG(block.is_null(), false, "Block cannot be null");
    return ft_in_area(block->block, area->rect);
}

bool FTSimState::is_solved(const Ref<FTDesignSpec> spec) const {
    ERR_FAIL_COND_V_MSG(spec.is_null(), false, "Design spec cannot be null");
    return ft_is_solved(state, spec->spec);
}

int FTSimState::get_tick() const { return state->tick; }

TypedArray<FTBlock> FTSimState::get_level_blocks() const {
    TypedArray<FTBlock> result;
    for (size_t i = 0; i < state->design.level_blocks.size(); ++i) {
        Ref<FTBlock> block;
        block.instantiate();
        block->block = state->design.level_blocks[i];
        result.push_back(block);
    }
    return result;
}

TypedArray<FTBlock> FTSimState::get_design_blocks() const {
    TypedArray<FTBlock> result;
    for (size_t i = 0; i < state->design.design_blocks.size(); ++i) {
        Ref<FTBlock> block;
        block.instantiate();
        block->block = state->design.design_blocks[i];
        result.push_back(block);
    }
    return result;
}

TypedArray<FTJointStack> FTSimState::get_joint_stacks() const {
    TypedArray<FTJointStack> result;
    for (size_t i = 0; i < state->design.joint_stacks.size(); ++i) {
        Ref<FTJointStack> joint_stack;
        joint_stack.instantiate();
        joint_stack->joint_stack = state->design.joint_stacks[i];
        result.push_back(joint_stack);
    }
    return result;
}

String to_gd(std::string s) { return String(s.c_str()); }

std::string from_gd(String s) {
    std::string result;
    for (int i = 0; i < s.length(); ++i) {
        result += char(s[i]);
    }
    return result;
}

void FTBackend::_bind_methods() {
    BIND_CONSTANT(FT_NO_ID)
    BIND_CONSTANT(FT_NO_JOINT)
    BIND_CONSTANT(FT_NO_JOINT_STACK)

    BIND_CONSTANT(FT_MAX_JOINTS)

    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, STATIC_RECT);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, STATIC_CIRC);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, DYNAMIC_RECT);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, DYNAMIC_CIRC);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, GP_RECT);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, GP_CIRC);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, CW);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, CCW);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, UPW);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, WOOD);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, WATER);
    BIND_ENUM_CLASS_CONSTANT(FTSim, PieceType, SIZE);

    BIND_ENUM_CLASS_CONSTANT(FT, JointType, CW);
    BIND_ENUM_CLASS_CONSTANT(FT, JointType, UPW);
    BIND_ENUM_CLASS_CONSTANT(FT, JointType, CCW);

    ClassDB::bind_static_method("FTBackend", D_METHOD("math_hash"), &FTBackend::math_hash);
    ClassDB::bind_static_method("FTBackend", D_METHOD("dtostr", "value"), &FTBackend::dtostr);
    ClassDB::bind_static_method("FTBackend", D_METHOD("strtod", "value"), &FTBackend::strtod);
    ClassDB::bind_static_method("FTBackend", D_METHOD("get_assert_flags"),
                                &FTBackend::get_assert_flags);
    ClassDB::bind_static_method("FTBackend", D_METHOD("is_goal_object", "type"),
                                &FTBackend::is_goal_object);
    ClassDB::bind_static_method("FTBackend", D_METHOD("is_circle", "type"), &FTBackend::is_circle);
    ClassDB::bind_static_method("FTBackend", D_METHOD("is_wheel", "type"), &FTBackend::is_wheel);
    ClassDB::bind_static_method("FTBackend", D_METHOD("is_player_movable", "type"),
                                &FTBackend::is_player_movable);
    ClassDB::bind_static_method("FTBackend", D_METHOD("is_player_deletable", "type"),
                                &FTBackend::is_player_deletable);
}

String FTBackend::math_hash() { return to_gd(ft_math_hash()); }

String FTBackend::dtostr(double v) { return to_gd(ft_dtostr(v)); }

double FTBackend::strtod(String s) { return ft_strtod_elsenan(from_gd(s)); }

int FTBackend::get_assert_flags() {
    int result = 0;
    if (get_assert_flag()) {
        result |= 1;
    }
    if (get_assertmem_flag()) {
        result |= 2;
    }
    return result;
}

bool FTBackend::is_goal_object(FTSimPieceType::Type type) {
    return ::ft_is_goal_object(static_cast<ft_piece_type::type>(type));
}

bool FTBackend::is_circle(FTSimPieceType::Type type) {
    return ::ft_is_circle(static_cast<ft_piece_type::type>(type));
}

bool FTBackend::is_wheel(FTSimPieceType::Type type) {
    return ::ft_is_wheel(static_cast<ft_piece_type::type>(type));
}

bool FTBackend::is_player_movable(FTSimPieceType::Type type) {
    return ::ft_is_player_movable(static_cast<ft_piece_type::type>(type));
}

bool FTBackend::is_player_deletable(FTSimPieceType::Type type) {
    return ::ft_is_player_deletable(static_cast<ft_piece_type::type>(type));
}