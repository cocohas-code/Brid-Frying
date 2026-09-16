#include "MyPG.h"
#include "MyGameMain.h"

namespace Rule
{
	//ゲーム情報
	enum class page
	{
		p1,p2,p3,p4,non
	};

	enum class botan
	{
		Left,Right,Non
	};

	DG::Image::SP imgBG,img1, img2, img3, img4, imgL, imgR;
	DG::Font::SP FontA;
	int Page;
	const int Max_page = 4;

	page p;
	botan b;
	//初期化
	void  Initialize()
	{
		imgBG = DG::Image::Create("./data/image/imagekari/BG.png");
		img1 = DG::Image::Create("./data/image/rule/rule1.png");
		img2 = DG::Image::Create("./data/image/rule/rule2.png");
		img3 = DG::Image::Create("./data/image/rule/rule3.png");
		img4 = DG::Image::Create("./data/image/rule/rule4.png");
		imgL = DG::Image::Create("./data/image/rule/left.png");
		imgR = DG::Image::Create("./data/image/rule/right.png");
		FontA = DG::Font::Create("BIZ UDPゴシック", 20, 40);
		Page = 1;
	}
	
	void  Finalize()
	{
		img1.reset();
		img2.reset();
		img3.reset();
		img4.reset();
		imgL.reset();
		imgR.reset();
	
	}
	//更新処理
	TaskFlag  UpDate()
	{
		auto inp = ge->in1->GetState();
	
			if (inp.LStick.BL.down)
			{
				//左
				if (Page > 1)
				{
					Page--;
					p = page::non;
				}
			}
			else if (inp.LStick.BR.down)
			{
				//右
				
				if (Page < Max_page)
				{
					Page++;
					p = page::non;
				}
			}
		
		//ページの一覧
		switch (Page)
		{
		case 1:p = page::p1;break;
		case 2:p = page::p2;break;
		case 3:p = page::p3;break;
		case 4:p = page::p4;break;
		default:p = page::non;
			break;
		}

		TaskFlag rtv = TaskFlag::Rule;//取りあえず現在のタスクを指定
			if (true == inp.ST.down) {
				rtv = TaskFlag::Title;	//次のタスク
			}
			return rtv;
	}
	//描画処理
	void  Render()
	{
		//BG
		ML::Box2D draw(0, 0, 1270, 720);
		ML::Box2D src(0, 0, 1270, 720);
		imgBG->Draw(draw, src);

		//P1
		if (p == page::p1)
		{
			ML::Box2D draw(150, 20, 900, 630);
			ML::Box2D src(0, 0, 900, 630);
			img1->Draw(draw, src);
		}
		//P2
		if (p == page::p2)
		{
			ML::Box2D draw(150, 20, 900, 630);
			ML::Box2D src(0, 0, 900, 630);
			img2->Draw(draw, src);
		}
		//P3
		if (p == page::p3)
		{
			ML::Box2D draw(150, 20, 900, 630);
			ML::Box2D src(0, 0, 900, 630);
			img3->Draw(draw, src);
		}
		//P4
		if (p == page::p4)
		{
			ML::Box2D draw(150, 20, 900, 630);
			ML::Box2D src(0, 0, 900, 630);
			img4->Draw(draw, src);
		}
		
		if (Page != 4)
		{
			//右
			ML::Box2D drawR(1200, 350, 50, 100);
			ML::Box2D srcR(0, 0, 50, 100);
			imgR->Draw(drawR, srcR);
		}
		if (Page != 1)
		{
			//左
			ML::Box2D drawL(0, 350, 50, 100);
			ML::Box2D srcL(0, 0, 50, 100);
			imgL->Draw(drawL, srcL);
		}		
		ML::Box2D textBox1(0, 650, 300, 100);
		string text1 = "Sキーで戻る";
		FontA->Draw(textBox1, text1, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
	


}