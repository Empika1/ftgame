extends Node

@onready var render: FTRender = $FTRender

var scale: float = 0.5
var shift: Vector2 = Vector2(600, 400)
const maxScale = 100
const minScale = 0.01
func zoom(deltaScale: float, screen_pos: Vector2):
	var old_scale = scale
	scale *= deltaScale
	scale = clamp(scale, minScale, maxScale)
	shift -= (screen_pos - shift) * (scale / old_scale - 1)

func _unhandled_input(event): # TODO: remove
	if event is InputEventKey && event.pressed:
		if event.keycode == KEY_I:
			zoom(1.1, get_viewport().get_mouse_position())
		elif event.keycode == KEY_O:
			zoom(1. / 1.1, get_viewport().get_mouse_position())
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_WHEEL_DOWN:
			zoom(1. / 1.1, get_viewport().get_mouse_position())
		elif event.button_index == MOUSE_BUTTON_WHEEL_UP:
			zoom(1.1, get_viewport().get_mouse_position())

const layerMultimeshInstanceCount: int = 16384
const layerDataSize: Vector2i = Vector2(128, 128)
@onready var shaderMaterial: ShaderMaterial = load("res://render/render.tres")
@onready var mmAreas: MultiMeshInstance2D = $FTRender/mmAreas
@onready var mmBorders: MultiMeshInstance2D = $FTRender/mmBorders
@onready var mmInsides: MultiMeshInstance2D = $FTRender/mmInsides
@export var colors: PackedColorArray = (func() -> PackedColorArray:
	var arr: PackedColorArray
	arr.resize(FTRender.ObjType_SIZE)
	arr[FTRender.ObjType_STATIC_RECT_BORDER] = Color("#008009"); arr[FTRender.ObjType_STATIC_RECT_INSIDE] = Color("#00be01")
	arr[FTRender.ObjType_STATIC_CIRC_BORDER] = Color("#008009"); arr[FTRender.ObjType_STATIC_CIRC_INSIDE] = Color("#00be01")
	arr[FTRender.ObjType_DYNAMIC_RECT_BORDER] = Color("#c6560c"); arr[FTRender.ObjType_DYNAMIC_RECT_INSIDE] = Color("#f9da2f")
	arr[FTRender.ObjType_DYNAMIC_CIRC_BORDER] = Color("#c6560c"); arr[FTRender.ObjType_DYNAMIC_CIRC_INSIDE] = Color("#f9892f")
	arr[FTRender.ObjType_GP_RECT_BORDER] = Color("#bb6666"); arr[FTRender.ObjType_GP_RECT_INSIDE] = Color("#ff6666")
	arr[FTRender.ObjType_GP_CIRC_BORDER] = Color("#bb6666"); arr[FTRender.ObjType_GP_CIRC_INSIDE] = Color("#ff6666")
	arr[FTRender.ObjType_WOOD_BORDER] = Color("#b55900"); arr[FTRender.ObjType_WOOD_INSIDE] = Color("#6b3400")
	arr[FTRender.ObjType_WATER_BORDER] = Color("#ffffff"); arr[FTRender.ObjType_WATER_INSIDE] = Color("#0000ff")
	arr[FTRender.ObjType_CW_BORDER] = Color("#fc8003"); arr[FTRender.ObjType_CW_INSIDE] = Color("#ffec00"); arr[FTRender.ObjType_CW_DECAL] = Color("#fc8003")
	arr[FTRender.ObjType_CCW_BORDER] = Color("#d147a5"); arr[FTRender.ObjType_CCW_INSIDE] = Color("#ffcccc"); arr[FTRender.ObjType_CCW_DECAL] = Color("#d147a5")
	arr[FTRender.ObjType_UPW_BORDER] = Color("#0a69fd"); arr[FTRender.ObjType_UPW_INSIDE] = Color("#89fae3"); arr[FTRender.ObjType_UPW_DECAL] = Color("#4a69fd")
	arr[FTRender.ObjType_BUILD_BORDER] = Color("#7777ee"); arr[FTRender.ObjType_BUILD_INSIDE] = Color("#bcdbf9")
	arr[FTRender.ObjType_GOAL_BORDER] = Color("#bb6666"); arr[FTRender.ObjType_GOAL_INSIDE] = Color("#f19191")
	arr[FTRender.ObjType_JOINT_NORMAL] = Color("#838383"); arr[FTRender.ObjType_JOINT_WHEEL_CENTER] = Color("#ffffff");
	return arr
).call()
@export var cornerRadii: PackedFloat32Array = (func() -> PackedFloat32Array:
	var arr: PackedFloat32Array
	arr.resize(FTRender.ObjType_SIZE)
	arr.set(FTRender.ObjType_STATIC_RECT_BORDER, 3)
	arr.set(FTRender.ObjType_DYNAMIC_RECT_BORDER, 3)
	arr.set(FTRender.ObjType_GP_RECT_BORDER, 3)
	arr.set(FTRender.ObjType_WOOD_BORDER, 2)
	arr.set(FTRender.ObjType_WATER_BORDER, 2)
	arr.set(FTRender.ObjType_BUILD_BORDER, 2)
	arr.set(FTRender.ObjType_GOAL_BORDER, 2)
	return arr
).call()
@export var borderThicknesses: PackedFloat32Array = (func() -> PackedFloat32Array:
	var arr: PackedFloat32Array
	arr.resize(FTRender.ObjType_SIZE)
	arr.set(FTRender.ObjType_STATIC_RECT_BORDER, 4)
	arr.set(FTRender.ObjType_STATIC_CIRC_BORDER, 4)
	arr.set(FTRender.ObjType_DYNAMIC_RECT_BORDER, 4)
	arr.set(FTRender.ObjType_DYNAMIC_CIRC_BORDER, 4)
	arr.set(FTRender.ObjType_GP_RECT_BORDER, 4)
	arr.set(FTRender.ObjType_GP_CIRC_BORDER, 4)
	arr.set(FTRender.ObjType_WOOD_BORDER, 3)
	arr.set(FTRender.ObjType_WATER_BORDER, 3)
	arr.set(FTRender.ObjType_BUILD_BORDER, 4)
	arr.set(FTRender.ObjType_GOAL_BORDER, 4)
	arr.set(FTRender.ObjType_CW_BORDER, 4)
	arr.set(FTRender.ObjType_CCW_BORDER, 4)
	arr.set(FTRender.ObjType_UPW_BORDER, 4)
	arr.set(FTRender.ObjType_JOINT_NORMAL, 2)
	arr.set(FTRender.ObjType_JOINT_WHEEL_CENTER, 2)
	return arr
).call()
@export var aaWidth: float = 0.5
@export var jointRadius: float = 8
@export var innerJointThresholdRadius: float = 40
@export var woodSizePadding: Vector2 = Vector2(-2, 2)
@export var waterSizePadding: Vector2 = Vector2(-2, 6)
@export var ghostRodPadding: float = 1

var spec: FTDesignSpec
var design: FTDesign
var settings: FTSimSettings = FTSimSettings.new()
var state: FTSimState
var setup: bool = false
func _ready() -> void:
	#init render
	render.initLayers(layerMultimeshInstanceCount, layerDataSize)
	render.initResources(shaderMaterial, mmAreas, mmBorders, mmInsides)
	render.initVisuals(colors, cornerRadii, borderThicknesses, aaWidth, jointRadius, innerJointThresholdRadius, woodSizePadding, waterSizePadding, ghostRodPadding)
	
	#get and parse design
	var xml = await Requests.retrieve_design(12709636, false)
	if !xml[0]:
		push_error("design retrieval failed")
		return
	var parsed_xml: Array = Parsing.parse_design(xml[1])
	if !parsed_xml[0]:
		push_error("design parsing failed")
		return
	spec = parsed_xml[1][5]

	design = FTDesign.new()
	design.init(spec)
	state = FTSimState.new()
	state.init(design, settings)
	setup = true

var frames: int = 0
var mod_ticks: int = 2
func _process(_delta: float) -> void:
	if not setup:
		return
	
	frames += 1
	if frames % mod_ticks == 0:
		state.step(settings)
	
	if state.is_solved(spec):
		print("WIN: ", state.get_tick(), " Ticks")
	else:
		print(state.get_tick(), " Ticks")
	
	# get updated sim state
	var level_blocks: Array[FTBlock] = state.get_level_blocks()
	var design_blocks: Array[FTBlock] = state.get_design_blocks()
	var build_area: FTRect = spec.get_build()
	var goal_area: FTRect = spec.get_goal()
	
	# prepare render
	render.resetRender()
	
	# push updated sim state to render
	render.addBuildArea(Vector2(build_area.x, build_area.y), Vector2(build_area.w, build_area.h), 0)
	render.addGoalArea(Vector2(goal_area.x, goal_area.y), Vector2(goal_area.w, goal_area.h), 0)
	var blockses: Array = [level_blocks, design_blocks]
	for blocks: Array[FTBlock] in blockses:
		for piece: FTBlock in blocks:
			render.addPiece(piece.type as int, Vector2(piece.x, piece.y), Vector2(piece.w, piece.h), piece.angle)
	
	# render it!
	render.render(scale, shift)
