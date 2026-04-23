/*1. 가방은 TArray를 통해서 만드시고요.
2. TMap은 저희 TArray에 담긴 아이템들을 확인하게 되면 그 아이템 정보를 Key값으로 아이템 정보들이 뜨도록 만들어주시고요.
3. TSet은 칭호 획득을 사용하시면됩니다.
4. 그래서 해당 칭호가 있어야 아이템을 사용할 수 있도록 만들어주세요.*/
#include "HWActor.h"

// Sets default values
AHWActor::AHWActor()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
   
	FItem Sword;
	Sword.ItemName = FString(TEXT("무한의 대검"));
	Sword.ItemDescription = FString(TEXT("치명타 데미지가 상승합니다."));
	Sword.RequiredTitle = FName(TEXT("내가 캐리할게"));
	ItemInfoMap.Emplace(1,Sword);
   
	FItem Shield;
	Shield.ItemName = FString(TEXT("무지개 방패"));
	Shield.ItemDescription = FString(TEXT("공격을 튕겨냅니다."));
	Shield.RequiredTitle = FName(TEXT("무지개 반사~"));
	ItemInfoMap.Emplace(2, Shield);
   
	FItem Potion;
	Potion.ItemName = FString(TEXT("HP포션"));
	Potion.ItemDescription = FString(TEXT("체력을 150 회복합니다."));
	Potion.RequiredTitle = NAME_None;
	ItemInfoMap.Emplace(3, Potion);
}

// Called when the game starts or when spawned
void AHWActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Display, TEXT("========== [1] 아이템 획득 테스트 =========="));
	AddItem(1);
	AddItem(2);
	AddItem(3);
	AddItem(999);

	PrintInventory();

	UE_LOG(LogTemp, Display, TEXT("========== [2] 칭호 없이 사용 시도 =========="));
	UseItem(1);
	UseItem(2);
	UseItem(3);

	UE_LOG(LogTemp, Display, TEXT("========== [3] 칭호 획득 =========="));
	AcquireTitle(FName(TEXT("무지개 반사~")));
	AcquireTitle(FName(TEXT("내가 캐리할게")));

	UE_LOG(LogTemp, Display, TEXT("========== [4] 칭호 획득 후 재시도 =========="));
	UseItem(2);
	UseItem(1);

	PrintInventory();
}


// Called every frame
void AHWActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//아이템 추가
void AHWActor::AddItem(int32 ItemID)
{
   if (!ItemInfoMap.Contains(ItemID))
   {
   		
   	   UE_LOG(LogTemp, Warning, TEXT("[AddItem] 존재하지 않는 아이템 ID: %d"), ItemID);
	   if (GEngine)
	   {
	   		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow,
	   			FString::Printf(TEXT("[AddItem] 존재하지 않는 아이템 ID : %d"), ItemID));
	   }
   		return;
   }
	InventoryArray.Add(ItemID);
	UE_LOG(LogTemp, Log, TEXT("[AddItem] %s를 가방에 담음"), *ItemInfoMap.Find(ItemID)->ItemName);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green,
			FString::Printf(TEXT("[AddItem] %s를 가방에 담음."), *ItemInfoMap.Find(ItemID)->ItemName));
	}
}

//획득 칭호
void AHWActor::AcquireTitle(FName TitleName)
{
	if (TitleName == NAME_None)
	{
		return;
	}

	AcquiredTitleSet.Add(TitleName);
	UE_LOG(LogTemp, Log, TEXT("[칭호 획득] %s"), *TitleName.ToString());
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, 
			FString::Printf(TEXT("[칭호 획득] %s"), *TitleName.ToString()));
	}
}

//아이템 사용 (칭호 보유 여부 확인)
bool AHWActor::UseItem(int32 ItemID)
{
	if (!InventoryArray.Contains(ItemID))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UseItem] 가방에 해당 아이템이 없습니다."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("[UseItem] 가방에 해당 아이템이 없습니다."));
		}
		return false;
	}
	
	const FItem* ItemPtr = ItemInfoMap.Find(ItemID);
	if (!ItemPtr)
	{
		return false;
	}
	
	const bool bTitle = (ItemPtr->RequiredTitle != NAME_None);
	if (bTitle && !AcquiredTitleSet.Contains(ItemPtr->RequiredTitle))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UseItem] '%s' 사용 실패. 필요 칭호 : [%s]"),
		   *ItemPtr->ItemName, *ItemPtr->RequiredTitle.ToString());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
				FString::Printf(TEXT("[UseItem] '%s' 사용 실패. 필요 칭호 : [%s]"), *ItemPtr->ItemName, *ItemPtr->RequiredTitle.ToString()));
		}
		return false;
	}
	
	InventoryArray.RemoveSingle(ItemID);
	UE_LOG(LogTemp, Log, TEXT("[UseItem] '%s' 사용/장착"), *ItemPtr->ItemName);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, 
			FString::Printf(TEXT("[UseItem] '%s' 사용/장착"), *ItemPtr->ItemName));
	}
	return true;
}

//보유 아이템 출력	
void AHWActor::PrintInventory()
{
	if (!GEngine) return;
	UE_LOG(LogTemp, Log, TEXT("=== 보유 아이템 : %d개 ==="), InventoryArray.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,
		FString::Printf(TEXT("=== 보유 아이템 : %d개 ==="), InventoryArray.Num()));


	for (const int32 ItemID : InventoryArray)
	{
		if (const FItem* Data = ItemInfoMap.Find(ItemID))
		{
			UE_LOG(LogTemp, Log, TEXT(" - [%d] %s (%s)"), ItemID, *Data->ItemName, *Data->ItemDescription);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,
				FString::Printf(TEXT(" - [%d] %s (%s)"),
					ItemID, *Data->ItemName, *Data->ItemDescription));
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("=== 보유 칭호 : %d개 ==="), AcquiredTitleSet.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,
		FString::Printf(TEXT("=== 보유 칭호 : %d개 ==="), AcquiredTitleSet.Num()));
	
	for (const FName& Title : AcquiredTitleSet)
	{
		UE_LOG(LogTemp, Log, TEXT(" - %s"), *Title.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,
			FString::Printf(TEXT(" - %s"), *Title.ToString()));
	}
}
