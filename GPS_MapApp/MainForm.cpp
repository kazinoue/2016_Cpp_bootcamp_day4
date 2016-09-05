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
	TabControl1->ActiveTab = TabItem1;

    // MapView1 �̃Y�[����Ԃ̏����l�� 15 �ɐݒ肷��B
	MapView1->Zoom = 15;
}
//---------------------------------------------------------------------------
// Switch1 �������ꂽ�Ƃ��̏����B
void __fastcall TForm1::Switch1Switch(TObject *Sender)
{
	// �q���摜�ƒʏ�n�}�̐؂�ւ����s��
	if (Switch1->IsChecked) {
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
	try {
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
	} catch (...) {
		ListBoxGroupHeader1->Text = "Getcoder service error";
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
// Button1 �������ꂽ�Ƃ��̏����B
void __fastcall TForm1::Button1Click(TObject *Sender)
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

	// �����x�Z���T�[�Ɋւ�鏈��
	{
		// �X���\���p�̉~�̒��a�B
		const static int circleDiameter = 50;

		// �����x�Z���T�[�̒l�ɑ΂���␳�W��
		const static int accelCoefficient = 100;

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
		Label1->Text = UnicodeString().sprintf( L"X: %3.2f", AccelX );
		Label2->Text = UnicodeString().sprintf( L"Y: %3.2f", AccelY );
		Label3->Text = UnicodeString().sprintf( L"Z: %3.2f", AccelZ );
		Label4->Text = UnicodeString().sprintf( L"accel: %3.2f", syntheticAccel );

		// �����x�� + - �ɂ���ăt�H���g�̐F��ς���B
		// �O�����Z�q���g�p
		// (��) ? (true �̏ꍇ�̏���) : (false �̏ꍇ�̏���)
		AccelX >= 0
			? Label1->TextSettings->FontColor = TAlphaColorRec::Black
			: Label1->TextSettings->FontColor = TAlphaColorRec::Red;
		AccelY >= 0
			? Label2->TextSettings->FontColor = TAlphaColorRec::Black
			: Label2->TextSettings->FontColor = TAlphaColorRec::Red;
		AccelZ >= 0
			? Label3->TextSettings->FontColor = TAlphaColorRec::Black
			: Label3->TextSettings->FontColor = TAlphaColorRec::Red;

		// Circle �̒��a�����������x�ɍ��킹�ĕς���B
		Circle1->Width  = circleDiameter * syntheticAccel / accelCoefficient;
		Circle1->Height = Circle1->Width;

		// Circle �� x, y ���̉����x�̒l�ɍ��킹�� Grid �ɏd�˂ĕ\������B
		Circle1->Position->X = ((PlotGrid1->Width  - Circle1->Width )/2.0) - (AccelX*10.0);
		Circle1->Position->Y = ((PlotGrid1->Height - Circle1->Height)/2.0) + (AccelY*10.0);

		// Circle �̌v�Z��̈ʒu����ʂ̊O�ɂ͂ݏo�������ȏꍇ�́A
		// �͂ݏo���Ȃ��悤�Ɍv�Z���ʂ�␳����B
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

			// �␳�����������ꍇ�͉~�̐F��ԐF�ɕς���B
			if (outOfRange) Circle1->Stroke->Color = TAlphaColorRec::Red;
			else            Circle1->Stroke->Color = TAlphaColorRec::Black;
		}
		// �擾���������x���� Memo1 �Ƀ��O�Ƃ��ċL�^����B
//		Memo1->Lines->Insert(
//			0,
//			UnicodeString().sprintf(
//				L"%3.2f = X:%3.2f Y:%3.2f Z:%3.2f",
//				syntheticAccel, AccelX, AccelY, AccelZ )
//		);
	}

	// �O���t�`��Ɋւ��鏈���B
	{
		// �O���t�X�V���s��ꂽ�񐔂�ۑ����Ă����ϐ��B
		static int numCount = 0;

		// �O���t�̕`��͈͂̐ݒ�B
		// �ő�Œ���50����������`�悷��B
		const static int axis_x_limit = 50;

		// 3���̍��������x���O���t�iTeeChart�j�ɕ`�悷�鏈���B
		Chart1->Series[0]->AddY(syntheticAccel);
		Chart1->Series[1]->AddY(AccelX);
		Chart1->Series[2]->AddY(AccelY);
		Chart1->Series[3]->AddY(AccelZ);

		// 3���̍��������x�O���t�𒼋�10�b�� ( = 0.2 �b x 50�T���v�� �j�����`�悷�邽�߂̉����͈͂̒����B
		if ( numCount++ > axis_x_limit ) {
			Chart1->Axes->Bottom->SetMinMax(numCount - axis_x_limit, numCount);
		}
	}
}
//---------------------------------------------------------------------------
// �����x�Z���T�[�̒l�ǂݍ��݂� on / off �؂�ւ��B
void __fastcall TForm1::Switch2Switch(TObject *Sender)
{
	// Switch2 �̃X�e�[�^�X�����̂܂� Timer1 �� enabled / disabled �ɗp����B
	Timer1->Enabled = Switch2->IsChecked;
}
//---------------------------------------------------------------------------


