#include "MyPG.h"
#include "MyGameMain.h"
#include "player_damege.h"
#include "CntDown.h"
#include "score.h"

namespace Game
{
	//ゲーム情報
	DG::Image::SP  imgPlayer, imgEnemy, imgStar, imgBG, imgFin; //画像読み込み
	DG::Font::SP  fontA;									   //フォント
	int b1, b2;												   //背景スクロール座標
	int Limit;                                                 //カウントダウン
	bool GameOver;	                                           //ゲームオーバー判定

	//ゲームステータス　通常　接触　無
	enum class State { Normal, Hit, Non, };


	//構造体
	struct chara
	{
		State state;
		int x, y;
		ML::Box2D hitBase;
		int timeCnt;
		int timeMax;
		int moveX, moveY;
		int score_Hit;
		int score;
		int damege;
	};

	//関数のプロトタイプ宣言
	chara Player;
	void player_initialize(chara& c_);
	void player_UpData(chara& c_, int x, int y);
	void player_Render(chara& c_);
	chara Stars;
	void Star_initialize(chara& c_);
	void Star_UpData(chara& c_);
	void Star_Render(chara& c_);
	void Star_Appear(chara& c_);
	chara Enemy;
	void Enemy_initialize(chara& c_);
	void Enemy_UpDate(chara& c_);
	void Enemy_Render(chara& c_);





	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		//背景
		imgBG = DG::Image::Create("./data/image/imagekari/BG.png");
		fontA = DG::Font::Create("BIZ UDPゴシック", 40, 80);
		b1 = 0;
		b2 = -1270;
		Limit = 60 * 60;

		//敵
		Enemy_initialize(Enemy);
		//プレイヤー
		player_initialize(Player);

		//星
		//四つ
		Star_initialize(Stars);

		damege_Initialize();
		CntDown_Initialize();
		score_Initialize();

		GameOver = false;
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------

	void Finalize()
	{
		imgPlayer.reset();
		imgBG.reset();
		imgEnemy.reset();
		imgStar.reset();
		fontA.reset();
		damege_Finalize();
		CntDown_Finalize();
		score_Finalize();
	}
	//-----------------------------------------------------------------------------
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();
		TaskFlag rtv = TaskFlag::Game;


		//操作
		player_UpData(Player, 20, 20);
		//敵
		Enemy_UpDate(Enemy);
		//星
		//yonn個
		Star_UpData(Stars);


		//敵からプレイヤへのダメージ処理
		damege_UpDate();
		//カウントダウン処理
		CntDown_UpDate();

		//背景ループ
		b1 -= 20;
		if (b1 <= b2)
		{
			b1 = 0;
		}

		//カウントダウン
		if (Stars.state != State::Non &&
			Player.state != State::Non &&
			Enemy.state != State::Non)
		{
			Limit--;
			if (Limit <= 0)
			{
				//０になったとき、すべてを無効にする
				Stars.state = State::Non;
				Player.state = State::Non;
				Enemy.state = State::Non;
			}
		}

		//タスク切り替え
		if (GameOver == true)
		{
			rtv = TaskFlag::Ending;
		}
		else if (true == inp.ST.down)
		{
			rtv = TaskFlag::Title;
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理 ２Ｄ
	//-----------------------------------------------------------------------------
	void Render()
	{
		//BG
		ML::Box2D draw1(0, 0, 1270, 720);
		ML::Box2D src1(0, 0, 1270, 720);
		draw1.x += b1;
		imgBG->Draw(draw1, src1);

		draw1.x += 1270;
		imgBG->Draw(draw1, src1);

		if (Stars.state == State::Non &&
			Player.state == State::Non &&
			Enemy.state == State::Non)
		{
			ML::Box2D textBox(600, 600, 500, 400);
			string text = "GameClear! SでTitle";
			fontA->Draw(textBox, text, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
		}

		//プレイヤー
		player_Render(Player);

		//敵
		Enemy_Render(Enemy);

		//星
		Star_Render(Stars);

		//カウントダウン
		CntDown_Render2D();
	}
	//ーーーーーーーーーーーーーー
	//プレイヤー（ピンク鳥）
	//ーーーーーーーーーーーーーー
	//初期化------------------------------------------------------------------
	void player_initialize(chara& c_)
	{
		imgPlayer = DG::Image::Create("./data/image/imagekari/player.png");
		imgFin = DG::Image::Create("./data/image/imagekari/clear.png");
		c_.x = 300;
		c_.y = 200;
		c_.hitBase = ML::Box2D(0, 0, 200, 150);
		c_.damege = 3;
		c_.state = State::Normal;
		c_.timeCnt = 0;
		c_.timeMax = 0;

	}
	//更新処理------------------------------------------------------------------
	void player_UpData(chara& c_, int x, int y)
	{
		TaskFlag rtv = TaskFlag::Game;
		auto inp = ge->in1->GetState();
		//プレイヤーの移動
		if (c_.state != State::Non)
		{
			if (inp.LStick.BR.on)
			{
				c_.x += x;
				if (c_.x > 1270)
				{
					c_.x = 1260;
				}
			}
			if (inp.LStick.BL.on)
			{
				c_.x -= x;
				if (c_.x < 0)
				{
					c_.x = 0;
				}
			}
			if (inp.LStick.BD.on)
			{
				c_.y += y;
				if (c_.y > 720)
				{
					c_.y = 720;
				}
			}
			if (inp.LStick.BU.on)
			{
				c_.y -= y;
				if (c_.y < 0)
				{
					c_.y = 0;
				}
			}
		//プレイヤーと敵の当たり判定
			if (c_.state == State::Normal)
			{
				ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
				ML::Box2D you = Enemy.hitBase.OffsetCopy(Enemy.x, Enemy.y);
				if (true == you.Hit(me))
				{
					c_.state = State::Hit;

					if (c_.damege > 1)
					{
						c_.damege -= 1;
					}
					else
					{
						GameOver = true;
					}
				}
			}
			if (c_.state == State::Hit)
			{
				c_.timeCnt++;
				if (c_.timeCnt > 30)
				{
					c_.timeCnt = 0;
					c_.state = State::Normal;
				}
			}

		}

	}
	//描画処理------------------------------------------------------------------
	void player_Render(chara& c_)
	{
		if (c_.state != State::Non)
		{
			ML::Box2D draw(-125, -100, 250, 200);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 250, 200);
			if (c_.state == State::Hit)
			{
				//当たったら点滅する
				if ((c_.timeCnt / 5) % 2 == 0)
				{
					src.w = 0;
					src.h = 0;
				}
			}
			imgPlayer->Draw(draw, src);
		}
		else
		{
			ML::Box2D drawf(300, 200, 600, 200);
			ML::Box2D srcf(0, 0, 600, 200);
			imgFin->Draw(drawf, srcf);
		}
		damege_Render2D(c_.damege);
	}
	//ーーーーーーーーーーーーーー
	//コイン（★）の処理
	//ーーーーーーーーーーーーーー
	//初期化------------------------------------------------------------------
	void Star_initialize(chara& c_)
	{

		imgStar = DG::Image::Create("./data/image/imagekari/star.png");
		c_.x = -1300;
		c_.y = 0;
		c_.moveX = -15;
		c_.moveY = 0;
		c_.hitBase = ML::Box2D(0, 0, 200, 160);
		c_.state = State::Normal;
		c_.timeCnt = 0;
		c_.timeMax = 120;
		c_.score = 0;
		c_.score_Hit = 50;
		srand((unsigned int)time(NULL));

	}
	//更新処理------------------------------------------------------------------
	void Star_UpData(chara& c_)
	{

		if (c_.state != State::Non)
		{
			Star_Appear(Stars);
			//当たり判定
			//プレイヤーと当たったら
			if (c_.state == State::Normal)
			{
				ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
				ML::Box2D you = Player.hitBase.OffsetCopy(Player.x, Player.y);
				if (true == you.Hit(me))
				{
					//星が消える
					c_.state = State::Hit;
					c_.score += c_.score_Hit;
				}

			}


		}
	}
	//星の生成処理------------------------------------------------------------------
	void Star_Appear(chara& c_)
	{

		if (c_.state != State::Non)
		{
			c_.timeCnt++;
			c_.x += c_.moveX;
			if (c_.timeCnt >= 120)
			{
				c_.timeCnt = 0;
				c_.state = State::Normal;
				c_.y = rand() % ((720 + 1) - 100);
				c_.x = 1300;
			}


		}


	}
	//描画処理------------------------------------------------------------------
	void Star_Render(chara& c_)
	{

		if (c_.state != State::Hit && c_.state != State::Non)
		{
			ML::Box2D draw1(-100, -80, 200, 160);
			draw1.Offset(c_.x, c_.y);
			ML::Box2D src1(0, 0, 200, 160);
			imgStar->Draw(draw1, src1);
		}


		score_Render2D(c_.score);

	}

	//ーーーーーーーーーーーーーー
	//敵（カラス）の処理
	//ーーーーーーーーーーーーーー
	//初期化------------------------------------------------------------------
	void Enemy_initialize(chara& c_)
	{

		imgEnemy = DG::Image::Create("./data/image/imagekari/karasu.png");
		c_.x = -1000;
		c_.y = 0;
		c_.moveX = 0;
		c_.timeCnt = 0;
		c_.timeMax = 0;
		c_.hitBase = ML::Box2D(0, 0, 200, 150);
		srand((unsigned int)time(NULL));
	}
	//更新処理------------------------------------------------------------------
	void Enemy_UpDate(chara& c_)
	{
		//敵の行動

		c_.timeCnt++;
		if (c_.timeCnt >= 300)
		{
			c_.timeCnt = 0;
			//右から左へ
			c_.x = 1270;
			c_.y = rand() % ((720 + 1) - 1);
			c_.moveX = -20;
			c_.moveY = 0;
		}


		c_.x += c_.moveX;
		c_.y += c_.moveY;
	}
	//描画処理------------------------------------------------------------------
	void Enemy_Render(chara& c_)
	{
		if (c_.state != State::Non)
		{
			ML::Box2D draw(-125, -104, 250, 208);
			ML::Box2D src(0, 0, 250, 208);
			draw.x += c_.x;
			draw.y += c_.y;
			imgEnemy->Draw(draw, src);
		}

	}
}