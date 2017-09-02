-- 文件名不能和表名冲突
object			= System.Object
Type			= System.Type
Object          = UnityEngine.Object
GameObject 		= UnityEngine.GameObject
Transform 		= UnityEngine.Transform
RectTransform   = UnityEngine.RectTransform
Rect   			= UnityEngine.Rect
MonoBehaviour 	= UnityEngine.MonoBehaviour
Component		= UnityEngine.Component
Application		= UnityEngine.Application
SystemInfo		= UnityEngine.SystemInfo
Screen			= UnityEngine.Screen
Camera			= UnityEngine.Camera
Material 		= UnityEngine.Material
Image   		= UnityEngine.UI.Image
--[[Renderer 		= UnityEngine.Renderer
AsyncOperation	= UnityEngine.AsyncOperation
CharacterController = UnityEngine.CharacterController
SkinnedMeshRenderer = UnityEngine.SkinnedMeshRenderer
Animation		= UnityEngine.Animation
AnimationClip	= UnityEngine.AnimationClip
AnimationEvent	= UnityEngine.AnimationEvent
AnimationState	= UnityEngine.AnimationState
Input			= UnityEngine.Input
KeyCode			= UnityEngine.KeyCode
AudioClip		= UnityEngine.AudioClip
AudioSource		= UnityEngine.AudioSource
Physics			= UnityEngine.Physics
Light			= UnityEngine.Light
LightType		= UnityEngine.LightType
ParticleEmitter	= UnityEngine.ParticleEmitter
Space			= UnityEngine.Space
CameraClearFlags= UnityEngine.CameraClearFlags
RenderSettings  = UnityEngine.RenderSettings
MeshRenderer	= UnityEngine.MeshRenderer
WrapMode		= UnityEngine.WrapMode
QueueMode		= UnityEngine.QueueMode
PlayMode		= UnityEngine.PlayMode
ParticleAnimator= UnityEngine.ParticleAnimator
TouchPhase 		= UnityEngine.TouchPhase
AnimationBlendMode = UnityEngine.AnimationBlendMode
SpriteRenderer = UnityEngine.SpriteRenderer--]]
SceneManager = UnityEngine.SceneManagement.SceneManager
WWW = UnityEngine.WWW

require "Define"
require "auto_script/z_client_struct_auto"
require "auto_script/z_client_process_auto"
require "auto_script/z_client_structrecv_auto"
require "auto_script/z_client_structsend_auto"
require "auto_script/z_client_netrecv_auto"
require "auto_script/z_client_netsend_auto"
require "config/auto_data_building_upgrade"
require "config/auto_data_city_tech"
require "config/auto_data_equip"
require "config/auto_data_equip_wash"
require "config/auto_data_equip_washrule"
require "config/auto_data_item"
require "config/auto_data_trainlong"
require "config/auto_data_trainqueue"
require "config/auto_data_upgrade"
require "config/auto_data_official_forging"
require "config/auto_data_official_gov"
require "config/auto_data_official_tech"
require "config/auto_data_story"
require "config/auto_data_nation_equip"
require "config/auto_data_hero"
require "config/auto_data_hero_colorup"
require "config/auto_data_hero_corpsmul"
require "config/auto_data_hero_skill"
require "config/auto_data_material_make"
require "config/auto_data_map_towninfo"
require "config/auto_data_map_zoneinfo"
require "config/auto_data_global"
require "config/conf_randname"
require "config/conf_city_tech_tree"
require "AskRecv"
require "AskSend"
require "GMCommand"
require "Award"
require "Game"
require "Network"
require "HttpRequest"
require "HttpResponse"
require "Common"
require "ResourceMgr"
require "CachePlayer"
require "CacheCity"
require "CacheHero"
require "CacheItem"
require "CacheEquip"
require "CacheQuest"
require "WorldMap"
require "WorldMapUnit"
require "WorldMapTile"
require "WorldMapMarchRoute"
require "uimod/LoginMod"
require "uimod/PopTextMod"
require "uimod/NotifyMod"
require "uimod/MapZoneTipsMod"
require "uimod/DialogFrameMod"
require "uimod/BuildingOpratorMod"
require "uimod/BuildingTimerMod"
require "uimod/BuildingFreeMod"
require "uimod/BuildingOverMod"
require "uimod/BuildingQuestMod"
require "uimod/LevyMod"
require "uimod/GuardCallMod"
require "uimod/MapClickMod"
require "uidlg/AlertDlg"
require "uidlg/BagDlg"
require "uidlg/BuildingCreateDlg"
require "uidlg/BuildingUpgradeDlg"
require "uidlg/BuildingGetDlg"
require "uidlg/BuyWorkerDlg"
require "uidlg/BattleDlg"
require "uidlg/CreateDlg"
require "uidlg/ChangeShapeDlg"
require "uidlg/ChangeNameDlg"
require "uidlg/ChatDlg"
require "uidlg/CityGuardDlg"
require "uidlg/CityTechDlg"
require "uidlg/CityTechTreeDlg"
require "uidlg/EquipForgingDlg"
require "uidlg/EquipWashDlg"
require "uidlg/GovInfoDlg"
require "uidlg/HeroDlg"
require "uidlg/HeroInfoDlg"
require "uidlg/HeroGetDlg"
require "uidlg/HeroListDlg"
require "uidlg/HeroTalkDlg"
require "uidlg/HeroExpDlg"
require "uidlg/HeroReplaceDlg"
require "uidlg/HeroWashDlg"
require "uidlg/HeroColorupDlg"
require "uidlg/ItemGetDlg"
require "uidlg/ItemUseDlg"
require "uidlg/ItemUseExDlg"
require "uidlg/JumpDlg"
require "uidlg/LevyDlg"
require "uidlg/MainDlg"
require "uidlg/MsgBoxDlg"
require "uidlg/MsgBoxExDlg"
require "uidlg/MaterialGetDlg"
require "uidlg/MaterialMakeDlg"
require "uidlg/MaterialMakeWillDlg"
require "uidlg/MapMainDlg"
require "uidlg/MapPosDlg"
require "uidlg/MapEnemyDlg"
require "uidlg/MapResDlg"
require "uidlg/MapBattleDlg"
require "uidlg/MapZoneDlg"
require "uidlg/MapGlobalDlg"
require "uidlg/NpcTalkDlg"
require "uidlg/NpcTalkOneDlg"
require "uidlg/OfficialHireDlg"
require "uidlg/QuestAwardDlg"
require "uidlg/QuickItemDlg"
require "uidlg/TrainDlg"
require "uidlg/PlayerDlg"
require "uidlg/StoreDlg"
require "uidlg/StoryDlg"

