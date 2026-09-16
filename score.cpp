#include "MyPG.h"
#include "score.h"

//ƒQ[ƒ€î•ñ
DG::Image::SP img1,imgsco;
char dispscore[5];

void  score_Initialize()
{
	img1 = DG::Image::Create("./data/image/imagekari/count.png");
	imgsco = DG::Image::Create("./data/image/imagekari/score.png");
}
void  score_Finalize()
{
	img1.reset();
	imgsco.reset();
}
void  score_UpDate()
{

}
void  score_Render2D(int x)
{
	//uscorev
	ML::Box2D draws(0, 0, 195, 65);
	ML::Box2D srcs(0, 0, 195, 65);
	imgsco->Draw(draws, srcs);

	//”
	snprintf(dispscore, 5, "%04d", x);
	for (int i = 0; i < 4; i++)
	{
		ML::Box2D draw(195, 0, 50, 75);
		ML::Box2D src((dispscore[i] - '0') * 50, 0, 50, 75);
		draw.Offset(i * 50 + 50, 0);
		img1->Draw(draw, src);
	}
}