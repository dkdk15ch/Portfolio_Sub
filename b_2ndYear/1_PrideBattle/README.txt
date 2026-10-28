PrideBattle プロジェクト案内

==================================================
1. プロジェクト概要
==================================================

プレイヤーが敵を倒しながらダストを集め、その数を敵と競う
三人称アクションゲームです。

==================================================
2. C++ソースコード
==================================================

場所	Source/BurnWithPride/

プレイヤー	Player/BurnWithPrideCharacter.h三人称キャラクターの移動、視点操作、ジャンプ入力を処理します。Enhanced Inputの入力を受け取り、Blueprintからも移動・視点・ジャンプを呼び出せます。近接攻撃、ダメージ処理、無敵状態、重力方向に合わせた移動を扱います。

Player/BurnWithPridePlayerController.h	プレイヤー入力の初期化と、モバイル操作用ウィジェットの設定を行います。

Player/PridePlayerState.h	プレイヤーのHP、死亡イベント、ダスト数を管理します。HPが0になったときのOnDeadをBlueprintへ通知します。

敵	Enemy/NPCBase.h敵キャラクターの基底クラスです。キャラクターのライフサイクル、毎フレーム処理、ダメージ受付をまとめ、HP減少処理をBlueprintイベントへ連携します。

弾（未採用）	Bullet/BulletBase.hプレイヤーを中心に旋回する弾の移動と、接触対象へのダメージ処理です。旋回半径、旋回速度、プレイヤー側ソケット名、ダメージ量を調整できます。

カウンター	Counter/ScoreCounter.hプレイヤー側と敵側のスコアを保持し、スコア差を取得します。Counter/DustCounter.hダスト数の加算、取得、リセットを行うBlueprint対応オブジェクトです。

重力制御	GravityController/GravityController.h重力方向が変化するステージ向けのPlayerControllerです。カメラとプレイヤーの回転を、現在の重力方向を基準に変換します。GetGravityRelativeRotation、GetGravityWorldRotationで変換処理を確認できます。

経路探索	PathFinding/PathGraphData.h経路グラフの頂点、隣接ノード、コストを定義するData Assetです。PathFinding/PathFinderComponent.hグラフ上でA*探索を行い、開始地点から目的地点までの経路と次の移動地点をBlueprintへ提供します。PathFinding/OBJParser.hOBJファイルを読み込み、頂点データを探索用ノードへ変換します。PathFinding/PathGraphDataImporter.hエディタ上でOBJからUPathGraphDataアセットを生成・保存するBlueprint関数です。エディタビルド時のみ利用されます。PathFinding/PathGraphImporterTester.h経路グラフのインポート処理を確認するテスト用クラスです。

戦闘インターフェース	Variant_Combat/Interfaces/CombatDamageable.hVariant_Combat/Interfaces/CombatAttacker.hダメージを受けるオブジェクトの共通インターフェースです。テンプレート流用
    
ゲーム進行	BurnWithPrideGameMode.hゲームモードのC++基底クラスです。BWP_GameStateBase.hゲーム全体で共有する状態を扱うGameStateです。


==================================================
3. Blueprint
==================================================

場所	Content/BurnWithPride/Blueprint/

ゲームモード・共通	BP_GameInstanceゲーム全体で共有する初期化・遷移用の設定です。BP_PrideGameModeゲームモードの共通設定を行うBlueprintです。BP_PrideGameMode_Titleタイトル画面のゲームモードと開始処理を構成します。BP_PrideGameMode_Ingameゲーム中のゲームモードとStateTreeの起動を構成します。

プレイヤー	Player/BP_BWPCharacterC++のBurnWithPrideCharacterを継承し、メッシュ、アニメーション、入力アセット、攻撃演出などを設定します。Player/BP_BWPPlayerStateC++のPridePlayerStateを継承し、HP・コンボ・ダストをUIと接続します。Player/BPI_Playerプレイヤー固有のBlueprint間連携に使用するインターフェースです。Player/BP_TitlePlayerタイトル画面で使用するプレイヤー設定です。

敵	NPC/BP_NPCC++のNPCBaseを継承した敵の見た目、ダメージ反応、StateTreeを設定します。NPC/APC_NPC_AStarA*経路探索を使用する敵の構成です。NPC/BP_EnemySpawner、BP_StationSpawner敵や敵ステーションの生成を管理します。NPC/BP_EnemyStation敵の拠点側の処理を構成します。NPC/BPC_FindArea敵がプレイヤーを発見する範囲の処理です。NPC/BPI_FoundPlayer、BPI_Obstacle、BPI_SpeedUp敵AIと周囲のオブジェクト間で使用するBlueprintインターフェースです。

ダスト	Dust/BP_Dustプレイヤーが取得するダストのアクターです。Dust/BP_DustCounterC++のDustCounterをBlueprintから扱うための構成です。Dust/BP_DustManager、BP_DustSpawnerダストの配置・生成・管理を行います。Dust/BP_AttractedDust2プレイヤーへ引き寄せられるダストの演出・挙動です。

経路探索・弾	AStar/BP_Ground、BP_PathNodeViewer経路グラフの配置確認とノード表示に使用します。AStar/Ground/PathNodesDataステージ上の経路ノードを保存したUPathGraphDataアセットです。Bullet/BPI_PlayerBullet弾とプレイヤー間のBlueprint連携に使用するインターフェースです。


==================================================
4. StateTree
==================================================

場所	Content/BurnWithPride/StateTree/

ゲームモード	GameMode/ST_GameModeタイトル、ゲーム開始、プレイ時間、結果画面への遷移を管理します。GameMode/STT_CreateWidget、STT_RemoveWiget画面ごとのWidget生成・削除を行います。GameMode/STT_CountPlayTimeゲーム中の時間を計測します。GameMode/STT_NextLevel、STT_GoResult、STT_GoResultInGameマップ遷移と結果画面への遷移を行います。GameMode/STT_ShowTransitionEffect、ST_PlayBGM、ST_QuitGame遷移演出、BGM再生、ゲーム終了を構成します。

NPCのA* AI	NPC/AStar/ST_Enemy敵の基本的な状態遷移です。NPC/AStar/Task/STT_InitPath経路探索の初期化を行います。NPC/AStar/Task/STT_GetRandomNodePosランダムな経路ノードを取得します。NPC/AStar/Task/STT_Wander、STT_Chase、STT_ReturnHome徘徊、プレイヤー追跡、拠点への帰還を行います。NPC/AStar/Task/STT_Attack、STT_Jump攻撃とジャンプの状態を実行します。NPC/AStar/Task/STGT_FindPlayer、STGT_CheckHPプレイヤー発見とHP状態の判定を行います。NPC/AStar/Task/STT_ChangeTarget、STT_GlobalBindingsターゲット変更とStateTreeのバインディングを構成します。

NPCスポナー・メタAI	NPC/Spawner/ST_MetaAIスポナー全体の状態遷移を管理します。NPC/Spawner/STT_SpawnEnemy、STT_PauseSpawnEnemy敵の生成と生成停止を行います。NPC/Spawner/STGT_CheckEnemyWipeOut、STGT_CheckScore敵の全滅とスコア条件を判定します。NPC/Spawner/BPC_SpawnRate敵の生成間隔を構成します。


==================================================
5. UI・マップ・入力
==================================================

UI	Content/BurnWithPride/UMG/InGame/WBP_HUD_InGame2、WBP_HPBur、WBP_PrideBurプレイヤーのHP、プライドゲージ、ゲーム中HUDを表示します。WBP_Score、WBP_ScoreBarスコアとスコアバーを表示します。WBP_Timer、WBP_Time、WBP_TimeGraph残り時間と時間グラフを表示します。Content/BurnWithPride/UMG/Title/WBP_HUD_Title、WBP_GameName、WBP_PressButtom、WBP_QuitGameタイトル画面と操作案内を構成します。Content/BurnWithPride/UMG/Result/WBP_HUD_Result、WBP_Result、WBP_ResultScoreゲーム結果とスコアを表示します。Content/BurnWithPride/UMG/Transition/WBP_GameStart、WBP_FinishGame、WBP_Loadingゲーム開始、終了、ロード時の遷移演出を構成します。

マップ	Content/BurnWithPride/Map/Title2.umapタイトル画面です。Content/BurnWithPride/Map/InGame2.umapプレイヤー、敵、ダスト、経路グラフを使用するゲーム本編です。Content/BurnWithPride/Map/Result.umap結果画面です。Content/BurnWithPride/Map/TransitionScreen.umapマップ遷移中に表示する画面です。

入力	Content/BurnWithPride/Input/IA_Attack攻撃入力です。IA_UsePrideGuageプライドゲージ使用入力です。IMC_Defaultゲーム中の入力マッピングです。
