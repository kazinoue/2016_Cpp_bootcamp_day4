//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Maps.hpp>
#include <FMX.Memo.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Sensors.Components.hpp>
#include <System.Sensors.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.ExtCtrls.hpp>
#include <FMX.Objects.hpp>
#include <FMXTee.Chart.hpp>
#include <FMXTee.Engine.hpp>
#include <FMXTee.Procs.hpp>
#include <FMXTee.Series.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TLocationSensor *LocationSensor1;
	TMapView *MapView1;
	TPanel *Panel1;
	TMemo *Memo1;
	TButton *Button1;
	TSwitch *Switch1;
	TTabControl *TabControl1;
	TTabItem *TabItem1;
	TTabItem *TabItem2;
	TTabItem *TabItem3;
	TListBox *ListBox1;
	TListBoxGroupHeader *ListBoxGroupHeader1;
	TListBoxItem *ListBoxItemLatitude;
	TListBoxItem *ListBoxItemLongitude;
	TListBoxGroupHeader *ListBoxGroupHeader2;
	TListBoxItem *ListBoxItemAdminArea;
	TListBoxItem *ListBoxItemCountryCode;
	TListBoxItem *ListBoxItemCountryName;
	TListBoxItem *ListBoxItemFeatureName;
	TListBoxItem *ListBoxItemLocality;
	TListBoxItem *ListBoxItemPostalCode;
	TListBoxItem *ListBoxItemSubAdminArea;
	TListBoxItem *ListBoxItemSubLocality;
	TListBoxItem *ListBoxItemSubThoroughfare;
	TListBoxItem *ListBoxItemThoroughfare;
	TMotionSensor *MotionSensor1;
	TTabItem *TabItem4;
	TPlotGrid *PlotGrid1;
	TLayout *Layout1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TTimer *Timer1;
	TCircle *Circle1;
	TLayout *Layout2;
	TLabel *Label4;
	TChart *Chart1;
	TLineSeries *Series1;
	TStyleBook *StyleBook1;
	TSwitch *Switch2;
	void __fastcall Switch1Switch(TObject *Sender);
	void __fastcall LocationSensor1LocationChanged(TObject *Sender, const TLocationCoord2D &OldLocation,
          const TLocationCoord2D &NewLocation);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Switch2Switch(TObject *Sender);



private:	// ユーザー宣言
	// 地図の中心位置を保存する変数
	TMapCoordinate mapCenter;

    // ジオコーディング関連
	TGeocoder *FGeocoder;
	void __fastcall OnGeocodeReverseEvent(TCivicAddress* const Address);

public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
