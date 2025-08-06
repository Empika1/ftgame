#ifndef FTSIMADAPTER_H
#define FTSIMADAPTER_H

#include "bind_enum_class_constant.h"
#include "ftlib.hpp"
#include "ftsim.h"
#include "gstr_adapter.hpp"

#include "core/object/ref_counted.h"
#include "core/variant/typed_array.h"

class FTRect : public RefCounted {
    GDCLASS(FTRect, RefCounted)

  protected:
    static void _bind_methods();

  public:
    ft_rect rect;

    static Ref<FTRect> init(double x, double y, double w, double h);

    void set_x(double x);
    double get_x() const;

    void set_y(double y);
    double get_y() const;

    void set_w(double w);
    double get_w() const;

    void set_h(double h);
    double get_h() const;
};

struct FTSimPieceType {
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
        SIZE = ft_piece_type::SIZE,
    };
};

class FTBlock;

class FTBlockSpec : public RefCounted {
    GDCLASS(FTBlockSpec, RefCounted)

  protected:
    static void _bind_methods();

  public:
    ft_block_spec spec;

    static Ref<FTBlockSpec> init(FTSimPieceType::Type type, uint16_t id, double x, double y,
                                 double w, double h, double angle, uint16_t j1, uint16_t j2);

    void set_type(FTSimPieceType::Type type);
    FTSimPieceType::Type get_type() const;

    void set_id(uint16_t id);
    uint16_t get_id() const;

    void set_x(double x);
    double get_x() const;

    void set_y(double y);
    double get_y() const;

    void set_w(double w);
    double get_w() const;

    void set_h(double h);
    double get_h() const;

    void set_angle(double angle);
    double get_angle() const;

    void set_joint_1(uint16_t joint);
    uint16_t get_joint_1() const;

    void set_joint_2(uint16_t joint);
    uint16_t get_joint_2() const;

    Ref<FTBlock> to_block() const;
};

class FTDesignSpec : public RefCounted {
    GDCLASS(FTDesignSpec, RefCounted)

  protected:
    static void _bind_methods();

  public:
    ft_design_spec spec;

    void set_blocks(const TypedArray<FTBlockSpec> blocks);
    TypedArray<FTBlockSpec> get_blocks() const;
    void set_blocks_packed(const PackedByteArray t, const PackedFloat64Array x,
                           const PackedFloat64Array y, const PackedFloat64Array w,
                           const PackedFloat64Array h, const PackedFloat64Array r,
                           const PackedInt32Array j1, const PackedInt32Array j2);
    PackedFloat64Array get_slice(int pi) const;

    void set_build(const Ref<FTRect> rect);
    Ref<FTRect> get_build() const;

    void set_goal(const Ref<FTRect> rect);
    Ref<FTRect> get_goal() const;
};

class FTBlock : public RefCounted {
    GDCLASS(FTBlock, RefCounted)

  protected:
    static void _bind_methods();

  public:
    ft_block block;

    void set_block_idx(uint16_t block_idx);
    uint16_t get_block_idx() const;

    void set_type(FTSimPieceType::Type type);
    FTSimPieceType::Type get_type() const;

    void set_x(double x);
    double get_x() const;

    void set_y(double y);
    double get_y() const;

    void set_w(double w);
    double get_w() const;

    void set_h(double h);
    double get_h() const;

    void set_angle(double angle);
    double get_angle() const;

    void set_joint_stack_idx(uint16_t js_idx, size_t i);
    uint16_t get_joint_stack_idx(size_t i);
    void set_joint_stack_idxs(const TypedArray<uint16_t> js_idxs);
    TypedArray<uint16_t> get_joint_stack_idxs() const;

    void set_joint_idx(uint16_t joint_idx, size_t i);
    uint16_t get_joint_idx(size_t i);
    void set_joint_idxs(const TypedArray<uint16_t> joint_idxs);
    TypedArray<uint16_t> get_joint_idxs() const;
};

struct FTJointType {
    enum Type : int8_t {
        CCW = ft_piece_type::CCW,
        UPW = ft_piece_type::UPW,
        CW = ft_piece_type::CW,
    };
};

class FTJoint : public RefCounted {
    GDCLASS(FTJoint, RefCounted)

  protected:
    static void _bind_methods();

  public:
    ft_joint joint;

    void set_block_idx(uint16_t block_idx);
    uint16_t get_block_idx() const;

    void set_joint_stack_idx(uint16_t joint_stack_idx);
    uint16_t get_joint_stack_idx() const;

    void set_joint_idx(uint16_t joint_idx);
    uint16_t get_joint_idx() const;
};

class FTJointStack : public RefCounted {
    GDCLASS(FTJointStack, RefCounted)

  protected:
    static void _bind_methods();

  public:
    ft_joint_stack joint_stack;

    void set_joint_stack_idx(uint16_t joint_stack_idx);
    uint16_t get_joint_stack_idx() const;

    void set_joint(const Ref<FTJoint> joint, size_t i);
    Ref<FTJoint> get_joint(size_t i) const;
    void set_joints(const TypedArray<FTJoint> joints);
    TypedArray<FTJoint> get_joints() const;

    void set_x(double x);
    double get_x() const;

    void set_y(double y);
    double get_y() const;
};

class FTDesign : public RefCounted {
    GDCLASS(FTDesign, RefCounted)

  protected:
    static void _bind_methods();

  public:
    std::shared_ptr<ft_design> design;

    FTDesign() : design(std::make_shared<ft_design>()) {}

    void init(const Ref<FTDesignSpec> spec);

    void set_level_block(const Ref<FTBlock> block, size_t i);
    Ref<FTBlock> get_level_block(size_t i) const;
    void set_level_blocks(const TypedArray<FTBlock> level_blocks);
    TypedArray<FTBlock> get_level_blocks() const;

    void set_design_block(const Ref<FTBlock> block, size_t i);
    Ref<FTBlock> get_design_block(size_t i) const;
    void set_design_blocks(const TypedArray<FTBlock> design_blocks);
    TypedArray<FTBlock> get_design_blocks() const;

    void set_joint_stack(const Ref<FTJointStack> joint_stack, size_t i);
    Ref<FTJointStack> get_joint_stack(size_t i) const;
    void set_joint_stacks(const TypedArray<FTJointStack> joint_stacks);
    TypedArray<FTJointStack> get_joint_stacks() const;

    void set_build(const Ref<FTRect> rect);
    Ref<FTRect> get_build() const;

    void set_goal(const Ref<FTRect> rect);
    Ref<FTRect> get_goal() const;
};

class FTSimSettings : public RefCounted {
    GDCLASS(FTSimSettings, RefCounted)

  protected:
    static void _bind_methods();

  public:
    ft_sim_settings settings;
};

class FTSimState : public RefCounted {
    GDCLASS(FTSimState, RefCounted)

  protected:
    static void _bind_methods();

  public:
    std::shared_ptr<ft_sim_state> state;

    FTSimState() : state(std::make_shared<ft_sim_state>()) {}

    void init(const Ref<FTDesign> design, const Ref<FTSimSettings> settings);

    void step(const Ref<FTSimSettings> settings);

    bool in_area(const Ref<FTBlock> block, const Ref<FTRect> area) const;

    bool is_solved(const Ref<FTDesignSpec> spec) const;

    int get_tick() const;

    TypedArray<FTBlock> get_level_blocks() const;

    TypedArray<FTBlock> get_design_blocks() const;

    TypedArray<FTJointStack> get_joint_stacks() const;
};

bool& get_assert_flag();
bool& get_assertmem_flag();

String to_gd(std::string);
std::string from_gd(String);

class FTBackend : public RefCounted {
    GDCLASS(FTBackend, RefCounted)

  protected:
    static void _bind_methods();

  public:
    static String math_hash();
    static String dtostr(double);
    static double strtod(String);
    static int get_assert_flags();
    static bool is_goal_object(FTSimPieceType::Type type);
    static bool is_circle(FTSimPieceType::Type type);
    static bool is_wheel(FTSimPieceType::Type type);
    static bool is_player_movable(FTSimPieceType::Type type);
    static bool is_player_deletable(FTSimPieceType::Type type);
};

VARIANT_ENUM_CAST(FTSimPieceType::Type); //TODO: figure out why these enum types can't be mentioned in godot
VARIANT_ENUM_CAST(FTJointType::Type);

#endif // FTSIMADAPTER_H