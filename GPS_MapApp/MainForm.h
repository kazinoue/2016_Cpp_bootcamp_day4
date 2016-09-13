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
	TMemo *Memo1;
	TButton *ButtonShowCurrentLocation;
	TSwitch *SwitchMapView;
	TTabControl *TabControl1;
	TTabItem *TabItem1MapView;
	TTabItem *TabItem4Detail;
	TTabItem *TabItem2GeoCoder;
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
	TTabItem *TabItem3MotionSensor;
	TPlotGrid *PlotGrid1;
	TLayout *Layout2;
	TLabel *Label1AccelX;
	TLabel *Label2AccelY;
	TLabel *Label3AccelZ;
	TTimer *Timer1;
	TCircle *CircleTiltSensor;
	TLayout *Layout3;
	TLabel *LabelSyntheticAccel;
	TChart *ChartAccel;
	TLineSeries *SyntheticAccel;
	TStyleBook *StyleBook1;
	TSwitch *SwitchLocationSensor;
	TLineSeries *X;
	TLineSeries *Y;
	TLineSeries *Z;
	TLayout *Layout1;
	void __fastcall SwitchMapViewSwitch(TObject *Sender);
	void __fastcall LocationSensor1LocationChanged(TObject *Sender, const TLocationCoord2D &OldLocation,
          const TLocationCoord2D &NewLocation);
	void __fastcall ButtonShowCurrentLocationClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall SwitchLocationSensorSwitch(TObject *Sender);



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
