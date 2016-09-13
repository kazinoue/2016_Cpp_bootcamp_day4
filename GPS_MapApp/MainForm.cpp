//---------------------------------------------------------------------------
// �G���o�J�f���E�e�N�m���W�[�Y
// C++ Boot Camp Day4 (2016/09/01)
// �uC++Builder���o�C���A�v���J�����n�߁v�Ŏg�p�����T���v���A�v���P�[�V����
//
// ����:
// ���̃T���v���Ɋ܂܂�� MapView �R���|�[�l���g��
// Android �Ŏ��s����ꍇ�ɂ� Google Maps Android API ������ APIkey ���K�v�ł��B
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
	// �A�v���̋N������ TabItem1 ���K���\�������悤�ɂ��Ă����B
	TabControl1->ActiveTab = TabItem1MapView;
}
//---------------------------------------------------------------------------
// SwitchMapView �������ꂽ�Ƃ��̏����B
void __fastcall TForm1::SwitchMapViewSwitch(TObject *Sender)
{
	// �q���摜�ƒʏ�n�}�̐؂�ւ����s��
	if (SwitchMapView->IsChecked) {
		MapView1->MapType = TMapType::Satellite;
	}
	else {
		MapView1->MapType = TMapType::Normal;
	}
}
//---------------------------------------------------------------------------
// �ʒu��񂪕ω������ۂ̏���
void __fastcall TForm1::LocationSensor1LocationChanged(TObject *Sender, const TLocationCoord2D &OldLocation,
		  const TLocationCoord2D &NewLocation)
{
	// �v�������ܓx�o�x�� Debug �p�� Memo �ɏo�͂���B
	UnicodeString LocationString = UnicodeString().sprintf( L"%2.6f, %2.6f", NewLocation.Latitude, NewLocation.Longitude );
	Memo1->Lines->Insert(0,LocationString);

	// �v�������ܓx�o�x�� ListBox ���� Latitude, Longitude �ɂ��\������B
	ListBoxItemLatitude->ItemData->Detail  = UnicodeString().sprintf(L"%2.6f", NewLocation.Latitude);
	ListBoxItemLongitude->ItemData->Detail = UnicodeString().sprintf(L"%2.6f", NewLocation.Longitude);

	// �n�}�̌��݈ʒu��������������B
	mapCenter = TMapCoordinate::Create(NewLocation.Latitude,NewLocation.Longitude);
	MapView1->Location = mapCenter;

	// ���݂̈ܓx�o�x�ɑΉ�����Z�����擾���邽�߂̈�A�̏����B
	{
		// TGeocoder �̏������Ɛ������s���B
		if ( FGeocoder == NULL ) {
			if( TGeocoder::Current != NULL ) {
				FGeocoder = (TGeocoder*)new TGeocoderClass(TGeocoder::Current);
			}
		}

		// �Z���擾�������̃C�x���g���`����B
		if ( FGeocoder != NULL ) {
			FGeocoder->OnGeocodeReverse = OnGeocodeReverseEvent;
		}

		// ���݈ʒu����̏Z���擾�i�t���R�[�f�B���O�j�̎��s�B
		// ���s��̌��ʂ̓C�x���g�쓮�ŏ�������B
		if ( (FGeocoder != NULL) && (FGeocoder->Geocoding) ) {
			FGeocoder->GeocodeReverse(NewLocation);
		}
	}
}
//---------------------------------------------------------------------------
// �t�W�I�R�[�f�B���O���s���̏���
void __fastcall TForm1::OnGeocodeReverseEvent(TCivicAddress* const Address)
{
	// �ܓx�o�x���猻�݈ʒu�̏Z�����擾�ł����ꍇ�͕\�����X�V����B
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
// ButtonShowCurrentLocation �������ꂽ�Ƃ��̏����B
void __fastcall TForm1::ButtonShowCurrentLocationClick(TObject *Sender)
{
	// �n�}�̕\�������݈ʒu���S�ɂ���B
	MapView1->Location = mapCenter;
}
//---------------------------------------------------------------------------
// ��莞�Ԃ��Ƃɍs�������B
// �����ł� MotionSensor �̒l�Ɋւ��������s���Ă���B
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	// �����x�B
	// ���̕ϐ��͒l�̎擾�ƃO���t�`��̗����Ŏg���̂ŁA
	// �����̏�������Q�Ƃł���X�R�[�v�Ő錾����B
	double syntheticAccel;
	double AccelX;
	double AccelY;
	double AccelZ;

	// �����x�Z���T�[�̒l�ɑ΂���␳�W��
	const static int accelCoefficient = 100;

	// �����x�Z���T�[�Ɋւ�鏈��
	{
		// �X���\���p�̉~�̒��a�B
		const static int circleDiameter = 50;

		// x,y,z���̉����x���擾����B
		AccelX = MotionSensor1->Sensor->AccelerationX * accelCoefficient;
		AccelY = MotionSensor1->Sensor->AccelerationY * accelCoefficient;
		AccelZ = MotionSensor1->Sensor->AccelerationZ * accelCoefficient;

		// 3���̍��������x���Z�o����B
		// ����͉����x�x�N�g���̑傫���i�X�J���[�����j���������o�������B
		// 3���̉����x���ω����Ă����������x�ɕω����Ȃ���΁A
		// ���̂̉^���͕ω����Ă��Ȃ��Ɣ��f�ł���B
		syntheticAccel = sqrt( pow(AccelX,2) + pow(AccelY,2) + pow(AccelZ,2) );

		// �擾�����l�����x���ɏo�͂���B
		Label1AccelX->Text = UnicodeString().sprintf( L"X: %3.2f", AccelX );
		Label2AccelY->Text = UnicodeString().sprintf( L"Y: %3.2f", AccelY );
		Label3AccelZ->Text = UnicodeString().sprintf( L"Z: %3.2f", AccelZ );
		LabelSyntheticAccel->Text = UnicodeString().sprintf( L"accel: %3.2f", syntheticAccel );

		// �����x�� + - �ɂ���ăt�H���g�̐F��ς���B
		// �O�����Z�q���g�p
		// (��) ? (true �̏ꍇ�̏���) : (false �̏ꍇ�̏���)
		AccelX >= 0
			? Label1AccelX->TextSettings->FontColor = TAlphaColorRec::Black
			: Label1AccelX->TextSettings->FontColor = TAlphaColorRec::Red;
		AccelY >= 0
			? Label2AccelY->TextSettings->FontColor = TAlphaColorRec::Black
			: Label2AccelY->TextSettings->FontColor = TAlphaColorRec::Red;
		AccelZ >= 0
			? Label3AccelZ->TextSettings->FontColor = TAlphaColorRec::Black
			: Label3AccelZ->TextSettings->FontColor = TAlphaColorRec::Red;

		// Circle �̒��a�����������x�ɍ��킹�ĕς���B
		CircleTiltSensor->Width  = circleDiameter * syntheticAccel / accelCoefficient;
		CircleTiltSensor->Height = CircleTiltSensor->Width;

		// Circle �� x, y ���̉����x�̒l�ɍ��킹�� Grid �ɏd�˂ĕ\������B
		CircleTiltSensor->Position->X = ((PlotGrid1->Width  - CircleTiltSensor->Width )/2.0) - (AccelX*10.0);
		CircleTiltSensor->Position->Y = ((PlotGrid1->Height - CircleTiltSensor->Height)/2.0) + (AccelY*10.0);

		// Circle �̌v�Z��̈ʒu����ʂ̊O�ɂ͂ݏo�������ȏꍇ�́A
		// �͂ݏo���Ȃ��悤�Ɍv�Z���ʂ�␳����B
		{
			bool outOfRange = false;

			if ( CircleTiltSensor->Position->X < 0 ) {
				CircleTiltSensor->Position->X = 0;
				outOfRange = true;
			}
			else if (CircleTiltSensor->Position->X > PlotGrid1->Width - CircleTiltSensor->Width) {
				CircleTiltSensor->Position->X = PlotGrid1->Width - CircleTiltSensor->Width;
				outOfRange = true;
			}

			if ( CircleTiltSensor->Position->Y < 0 ) {
				CircleTiltSensor->Position->Y = 0;
				outOfRange = true;
			}
			else if (CircleTiltSensor->Position->Y > PlotGrid1->Height - CircleTiltSensor-> Height) {
				CircleTiltSensor->Position->Y = PlotGrid1->Height - CircleTiltSensor-> Height;
				outOfRange = true;
			}

			// �␳�����������ꍇ�͉~�̐F��ԐF�ɕς���B
			if (outOfRange) CircleTiltSensor->Stroke->Color = TAlphaColorRec::Red;
			else            CircleTiltSensor->Stroke->Color = TAlphaColorRec::Black;
		}
	}

	// �O���t�`��Ɋւ��鏈���B
	{
		// �O���t�X�V���s��ꂽ�񐔂�ۑ����Ă����ϐ��B
		static int numCount = 0;

		// �O���t�̕`��͈͂̐ݒ�B
		// �ő�Œ���50����������`�悷��B
		const static int axis_x_limit = 50;

		// �O���t���X�N���[�����邩�ǂ���
		const static bool graphScroll = false;

		if ( graphScroll == true ) {
		  // �O���t���X�N���[����������B
		  // �ʏ�͂������p�����ق������h�����悢���AWeb�Z�~�i�[�ł͎g��Ȃ������ŁB
		  ChartAccel->Axes->Bottom->SetMinMax(numCount - axis_x_limit, numCount);
		}
		else {
		  // �O���t���X�N���[�������Ƀy�[�W���O�����
		  // GoToWebiner ������z�肵�������B
			if ( numCount % axis_x_limit == 0 ) {
				ChartAccel->Series[0]->Clear();
				ChartAccel->Series[1]->Clear();
				ChartAccel->Series[2]->Clear();
				ChartAccel->Series[3]->Clear();
				ChartAccel->Axes->Bottom->SetMinMax( 0, axis_x_limit );
				ChartAccel->Axes->Left->SetMinMax( -(accelCoefficient*2), accelCoefficient*2 );
			}
		}

		numCount++;

		// 3���̉����x�⍇�������x���O���t�iTeeChart�j�ɕ`�悷�鏈���B
		ChartAccel->Series[0]->AddY(syntheticAccel);
		ChartAccel->Series[1]->AddY(AccelX);
		ChartAccel->Series[2]->AddY(AccelY);
		ChartAccel->Series[3]->AddY(AccelZ);

	}
}
//---------------------------------------------------------------------------
// �����x�Z���T�[�̒l�ǂݍ��݂� on / off �؂�ւ��B
void __fastcall TForm1::SwitchLocationSensorSwitch(TObject *Sender)
{
	// Switch2 �̃X�e�[�^�X�����̂܂� Timer1 �� enabled / disabled �ɗp����B
	Timer1->Enabled = SwitchLocationSensor->IsChecked;
}
//---------------------------------------------------------------------------


