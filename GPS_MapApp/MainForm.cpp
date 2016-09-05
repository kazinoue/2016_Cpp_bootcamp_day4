//---------------------------------------------------------------------------
// エンバカデロ・テクノロジーズ
// C++ Boot Camp Day4 (2016/09/01)
// 「C++Builderモバイルアプリ開発事始め」で使用したサンプルアプリケーション
//
// 注意:
// このサンプルに含まれる MapView コンポーネントは
// Android で実行する場合には Google Maps Android API 向けの APIkey が必要です。
// https://developers.google.com/maps/documentation/android-api/

#include <fmx.h>
#pragma hdrstop

#include "MainForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	// アプリの起動時は TabItem1 が必ず表示されるようにしておく。
	TabControl1->ActiveTab = TabItem1;

    // MapView1 のズーム状態の初期値を 15 に設定する。
	MapView1->Zoom = 15;
}
//---------------------------------------------------------------------------
// Switch1 が押されたときの処理。
void __fastcall TForm1::Switch1Switch(TObject *Sender)
{
	// 衛星画像と通常地図の切り替えを行う
	if (Switch1->IsChecked) {
		MapView1->MapType = TMapType::Satellite;
	}
	else {
		MapView1->MapType = TMapType::Normal;
	}
}
//---------------------------------------------------------------------------
// 位置情報が変化した際の処理
void __fastcall TForm1::LocationSensor1LocationChanged(TObject *Sender, const TLocationCoord2D &OldLocation,
		  const TLocationCoord2D &NewLocation)
{
	// 計測した緯度経度を Debug 用の Memo に出力する。
	UnicodeString LocationString = UnicodeString().sprintf( L"%2.6f, %2.6f", NewLocation.Latitude, NewLocation.Longitude );
	Memo1->Lines->Insert(0,LocationString);

	// 計測した緯度経度を ListBox 内の Latitude, Longitude にも表示する。
	ListBoxItemLatitude->ItemData->Detail  = UnicodeString().sprintf(L"%2.6f", NewLocation.Latitude);
	ListBoxItemLongitude->ItemData->Detail = UnicodeString().sprintf(L"%2.6f", NewLocation.Longitude);

	// 地図の現在位置情報を書き換える。
	mapCenter = TMapCoordinate::Create(NewLocation.Latitude,NewLocation.Longitude);
	MapView1->Location = mapCenter;

	// 現在の緯度経度に対応する住所を取得するための一連の処理。
	try {
		// TGeocoder の初期化と生成を行う。
		if ( FGeocoder == NULL ) {
			if( TGeocoder::Current != NULL ) {
				FGeocoder = (TGeocoder*)new TGeocoderClass(TGeocoder::Current);
			}
		}

		// 住所取得成功時のイベントを定義する。
		if ( FGeocoder != NULL ) {
			FGeocoder->OnGeocodeReverse = OnGeocodeReverseEvent;
		}

		// 現在位置からの住所取得（逆塩コーディング）の実行。
		// 実行後の結果はイベント駆動で処理する。
		if ( (FGeocoder != NULL) && (FGeocoder->Geocoding) ) {
			FGeocoder->GeocodeReverse(NewLocation);
		}
	} catch (...) {
		ListBoxGroupHeader1->Text = "Getcoder service error";
	}
}
//---------------------------------------------------------------------------
// 逆ジオコーディング実行時の処理
void __fastcall TForm1::OnGeocodeReverseEvent(TCivicAddress* const Address)
{

	// 緯度経度から現在位置の住所が取得できた場合は表示を更新する。
	if( Address != NULL ) {
		ListBoxItemAdminArea->ItemData->Detail       = Address->AdminArea;
		ListBoxItemCountryCode->ItemData->Detail     = Address->CountryCode;
		ListBoxItemCountryName->ItemData->Detail     = Address->CountryName;
		ListBoxItemFeatureName->ItemData->Detail     = Address->FeatureName;
		ListBoxItemLocality->ItemData->Detail        = Address->Locality;
		ListBoxItemPostalCode->ItemData->Detail      = Address->PostalCode;
		ListBoxItemSubAdminArea->ItemData->Detail    = Address->SubAdminArea;
		ListBoxItemSubLocality->ItemData->Detail     = Address->SubLocality;
		ListBoxItemSubThoroughfare->ItemData->Detail = Address->SubThoroughfare;
		ListBoxItemThoroughfare->ItemData->Detail    = Address->Thoroughfare;
	}
}
//---------------------------------------------------------------------------
// Button1 が押されたときの処理。
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	// 地図の表示を現在位置中心にする。
	MapView1->Location = mapCenter;
}
//---------------------------------------------------------------------------
// 一定時間ごとに行う処理。
// ここでは MotionSensor の値に関わる実装を行っている。
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	// 加速度。
	// この変数は値の取得とグラフ描画の両方で使うので、
	// 両方の処理から参照できるスコープで宣言する。
	double syntheticAccel;
	double AccelX;
	double AccelY;
	double AccelZ;

	// 加速度センサーに関わる処理
	{
		// 傾き表示用の円の直径。
		const static int circleDiameter = 50;

		// 加速度センサーの値に対する補正係数
		const static int accelCoefficient = 100;

		// x,y,z軸の加速度を取得する。
		AccelX = MotionSensor1->Sensor->AccelerationX * accelCoefficient;
		AccelY = MotionSensor1->Sensor->AccelerationY * accelCoefficient;
		AccelZ = MotionSensor1->Sensor->AccelerationZ * accelCoefficient;

		// 3軸の合成加速度を算出する。
		// これは加速度ベクトルの大きさ（スカラー成分）だけを取り出す処理。
		// 3軸の加速度が変化しても合成加速度に変化がなければ、
		// 物体の運動は変化していないと判断できる。
		syntheticAccel = sqrt( pow(AccelX,2) + pow(AccelY,2) + pow(AccelZ,2) );

		// 取得した値をラベルに出力する。
		Label1->Text = UnicodeString().sprintf( L"X: %3.2f", AccelX );
		Label2->Text = UnicodeString().sprintf( L"Y: %3.2f", AccelY );
		Label3->Text = UnicodeString().sprintf( L"Z: %3.2f", AccelZ );
		Label4->Text = UnicodeString().sprintf( L"accel: %3.2f", syntheticAccel );

		// 加速度の + - によってフォントの色を変える。
		// 三項演算子を使用
		// (式) ? (true の場合の処理) : (false の場合の処理)
		AccelX >= 0
			? Label1->TextSettings->FontColor = TAlphaColorRec::Black
			: Label1->TextSettings->FontColor = TAlphaColorRec::Red;
		AccelY >= 0
			? Label2->TextSettings->FontColor = TAlphaColorRec::Black
			: Label2->TextSettings->FontColor = TAlphaColorRec::Red;
		AccelZ >= 0
			? Label3->TextSettings->FontColor = TAlphaColorRec::Black
			: Label3->TextSettings->FontColor = TAlphaColorRec::Red;

		// Circle の直径を合成加速度に合わせて変える。
		Circle1->Width  = circleDiameter * syntheticAccel / accelCoefficient;
		Circle1->Height = Circle1->Width;

		// Circle を x, y 軸の加速度の値に合わせて Grid に重ねて表示する。
		Circle1->Position->X = ((PlotGrid1->Width  - Circle1->Width )/2.0) - (AccelX*10.0);
		Circle1->Position->Y = ((PlotGrid1->Height - Circle1->Height)/2.0) + (AccelY*10.0);

		// Circle の計算上の位置が画面の外にはみ出しそうな場合は、
		// はみ出さないように計算結果を補正する。
		{
			bool outOfRange = false;

			if ( Circle1->Position->X < 0 ) {
				Circle1->Position->X = 0;
				outOfRange = true;
			}
			else if (Circle1->Position->X > PlotGrid1->Width - Circle1->Width) {
				Circle1->Position->X = PlotGrid1->Width - Circle1->Width;
				outOfRange = true;
			}

			if ( Circle1->Position->Y < 0 ) {
				Circle1->Position->Y = 0;
				outOfRange = true;
			}
			else if (Circle1->Position->Y > PlotGrid1->Height - Circle1-> Height) {
				Circle1->Position->Y = PlotGrid1->Height - Circle1-> Height;
				outOfRange = true;
			}

			// 補正が発生した場合は円の色を赤色に変える。
			if (outOfRange) Circle1->Stroke->Color = TAlphaColorRec::Red;
			else            Circle1->Stroke->Color = TAlphaColorRec::Black;
		}
		// 取得した加速度情報を Memo1 にログとして記録する。
//		Memo1->Lines->Insert(
//			0,
//			UnicodeString().sprintf(
//				L"%3.2f = X:%3.2f Y:%3.2f Z:%3.2f",
//				syntheticAccel, AccelX, AccelY, AccelZ )
//		);
	}

	// グラフ描画に関する処理。
	{
		// グラフ更新が行われた回数を保存しておく変数。
		static int numCount = 0;

		// グラフの描画範囲の設定。
		// 最大で直近50件分だけを描画する。
		const static int axis_x_limit = 50;

		// 3軸の合成加速度をグラフ（TeeChart）に描画する処理。
		Chart1->Series[0]->AddY(syntheticAccel);
		Chart1->Series[1]->AddY(AccelX);
		Chart1->Series[2]->AddY(AccelY);
		Chart1->Series[3]->AddY(AccelZ);

		// 3軸の合成加速度グラフを直近10秒分 ( = 0.2 秒 x 50サンプル ）だけ描画するための横軸範囲の調整。
		if ( numCount++ > axis_x_limit ) {
			Chart1->Axes->Bottom->SetMinMax(numCount - axis_x_limit, numCount);
		}
	}
}
//---------------------------------------------------------------------------
// 加速度センサーの値読み込みの on / off 切り替え。
void __fastcall TForm1::Switch2Switch(TObject *Sender)
{
	// Switch2 のステータスをそのまま Timer1 の enabled / disabled に用いる。
	Timer1->Enabled = Switch2->IsChecked;
}
//---------------------------------------------------------------------------


