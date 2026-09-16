#include "CntDown.h"
#include "MyPG.h"

//ゲーム情報
DG::Image::SP img,imgcoron;
int Second;
int minute;
char dispsecond[4];
char dispminute[4];
//-----------------------------------------------------------------------------
//初期化処理
//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
//-----------------------------------------------------------------------------
void  CntDown_Initialize()
{
	img = DG::Image::Create("./data/image/imagekari/count.png");
	imgcoron = DG::Image::Create("./data/image/imagekari/count.png");
	Second = 60 * 60;
	minute = 0;
}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
void  CntDown_Finalize()
{
	img.reset();
	imgcoron.reset();
}
//-----------------------------------------------------------------------------
//更新処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void  CntDown_UpDate()
{
	Second--;
	if (Second <= 0)
	{
		Second = 0;
	}
}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理 ２Ｄ
//-----------------------------------------------------------------------------
void  CntDown_Render2D()

{
	//分
	snprintf(dispminute, 4, "%01d", minute);
	for (int i = 0; i < 1; i++)
	{
		ML::Box2D draw(970, 0, 50, 75);
		ML::Box2D src((dispminute[i] - '0') * 50, 0, 50, 75);
		draw.Offset(i * 50 + 50, 0);
		img->Draw(draw, src);
	}
	//秒
	snprintf(dispsecond, 4, "%02d", Second/60);
	for (int i = 0; i < 2; i++)
	{
		ML::Box2D draw(1070, 0, 50, 75);
		ML::Box2D src((dispsecond[i] - '0') * 50, 0, 50, 75);
		draw.Offset(i * 50 + 50, 0);
		img->Draw(draw, src);
	}
	ML::Box2D draw(1060, 0, 50, 75);
	ML::Box2D src(500, 0, 50, 75);
	imgcoron->Draw(draw, src);
}
