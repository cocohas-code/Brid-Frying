#include "MyPG.h"
#include "MyGameMain.h"

//タイトル画面
namespace Title
{
	
	//ゲーム情報
	DG::Image::SP   img,imgBR,imglogo,imgB;
	DG::Font::SP	fSTATE, fRULE,fbotan;

	float fadeAlpha;
	bool isfadingIn;
	
	//ボタンの状態
	enum class State {
		Left, Right,Non
	};

	struct Botan {
		State state;
		int x;
	};
	
	Botan botan;
	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		imgBR = DG::Image::Create("./data/image/imagekari/brackk.jpg");
		img = DG::Image::Create("./data/image/imagekari/BG.png");
		imglogo = DG::Image::Create("./data/image/imagekari/logo1.png");
		imgB = DG::Image::Create("./data/image/imagekari/star.png");


		fadeAlpha = 1.0f;
		isfadingIn = true;

		//シーン切り替えを分かりやすくするために入れた
		ge->dgi->EffectState().param.bgColor = ML::Color(0, 0, 0, 0);

		int x = 0;
		fSTATE = DG::Font::Create("BIZ UDPゴシック", 40, 80);
		fRULE = DG::Font::Create("BIZ UDPゴシック", 40, 80);
		fbotan = DG::Font::Create("BIZ UDPゴシック", 20, 40);
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		img.reset();
		imgBR.reset();
		imglogo.reset();
		imgB.reset();
		fSTATE.reset();
		fRULE.reset();
	}
	//-----------------------------------------------------------------------------
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		auto inp = ge->in1->GetState();
		TaskFlag rtv = TaskFlag::Title;//取りあえず現在のタスクを指

		if (isfadingIn)
		{
			fadeAlpha -= 0.1f;
			if (fadeAlpha <= 0)
			{
				fadeAlpha = 0;
				isfadingIn = false;
			}
		}

		//右…STATE
		//左…あそびかた　（未実装
		if (inp.LStick.BR.on)
		{
			botan.state = State::Right;

		}
		if (inp.LStick.BL.on)
		{
			botan.state = State::Left;
		}
		
		

	
		if (botan.state == State::Left)
		{
			//ゲーム開始
			if (true == inp.SE.down) {
				rtv = TaskFlag::Game;
			}
		}
		if (botan.state == State::Right)
		{
			//ルール説明へ（まだ仮）
			if (true == inp.SE.down) {
				rtv = TaskFlag::Rule;
			}
		}

		return rtv;

	
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{


		//背景(仮
		ML::Box2D  draw(0, 0, 1270, 720);
		ML::Box2D  src(0, 0, 1270, 720);
		img->Draw(draw, src);

			//ロゴ
			ML::Box2D draw1(250, 50, 660, 440);
			ML::Box2D src1(0, 0, 660, 440);
			imglogo->Draw(draw1, src1);

			//ボタン
			ML::Box2D draw2(40, 500, 200, 160);
			ML::Box2D src2(0, 0, 200, 160);
			if (botan.state == State::Left)
			{
				imgB->Draw(draw2, src2);
			}
			else if (botan.state == State::Right)
			{
				draw2.x += 600;
				imgB->Draw(draw2, src2);
			}

			ML::Box2D draw3(0, 0, 1270, 720);
			ML::Box2D src3(0, 0, 1270, 720);
			imgBR->Draw(draw3, src3, ML::Color(fadeAlpha, 1.0f, 1.0f, 1.0f));

			ML::Box2D textBox(200, 550, 400, 200);
			string text = "START";
			fSTATE->Draw(textBox, text, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));

			ML::Box2D textBox1(800, 550, 400, 200);
			string text1 = "あそびかた";
			fRULE->Draw(textBox1, text1, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));

			ML::Box2D textBox2(1070, 650, 300, 100);
			string text2 = "Aキーでけってい";
			fbotan->Draw(textBox2, text2, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
			
	}
}