#include "UI/APITest/APITestOverlay.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/API/ListFleets/FleetId.h"
#include "UI/API/ListFleets/ListFleetsBox.h"
#include "UI/APITest/APITestManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UAPITestOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(APITestManagerClass);
	APITestManager = NewObject<UAPITestManager>(this, APITestManagerClass);

	check(ListFleetsBox);
	check(ListFleetsBox->Button_ListFleets);
	// Add: 델리게이트에 함수 포인터를 직접 추가 (C++에서만 사용, 바인딩 해제 불가)
	// AddDynamic: UFUNCTION으로 선언된 함수 바인딩 (블루프린트/가비지 컬렉션 지원, 바인딩 해제 가능)
	ListFleetsBox->Button_ListFleets->OnClicked.AddDynamic(
		this,
		&UAPITestOverlay::ListFleetsButtonClicked);
}

void UAPITestOverlay::ListFleetsButtonClicked()
{
	check(APITestManager);

	// ListFleets API 응답을 받고 나서 호출할 함수 등록
	APITestManager->OnListFleetsResponseReceived.AddDynamic(this, &UAPITestOverlay::OnListFleetsResponseReceived);

	// ListFleets API 호출
	APITestManager->ListFleets();

	ListFleetsBox->Button_ListFleets->SetIsEnabled(false);
}

void UAPITestOverlay::OnListFleetsResponseReceived(const FDSListFleetsResponse& ListFleetsResponse, bool bWasSuccessful)
{
	if (APITestManager->OnListFleetsResponseReceived.IsAlreadyBound(this, &UAPITestOverlay::OnListFleetsResponseReceived))
	{
		APITestManager->OnListFleetsResponseReceived.RemoveDynamic(this, &UAPITestOverlay::OnListFleetsResponseReceived);
	}
	
	ListFleetsBox->ScrollBox_ListFleets->ClearChildren();
	
	if (bWasSuccessful)
	{
		for (const FString& FleetId : ListFleetsResponse.FleetIds)
		{
			UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
			FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString(FleetId));
			ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
		}
	}
	else
	{
		UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
		FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString("Something went wrong!"));
		ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
	}

	ListFleetsBox->Button_ListFleets->SetIsEnabled(true);
}
