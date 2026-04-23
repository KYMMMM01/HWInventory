/*1. 가방은 TArray를 통해서 만드시고요.
2. TMap은 저희 TArray에 담긴 아이템들을 확인하게 되면 그 아이템 정보를 Key값으로 아이템 정보들이 뜨도록 만들어주시고요.
3. TSet은 칭호 획득을 사용하시면됩니다.
4. 그래서 해당 칭호가 있어야 아이템을 사용할 수 있도록 만들어주세요.*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HWActor.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()
   
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
	FString ItemName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
	FString ItemDescription;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item")
	FName RequiredTitle;
};

UCLASS()
class HWINVENTORY_API AHWActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHWActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//아이템 ID
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory" )
	TArray<int32> InventoryArray;
	//아이템 정보
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory" )
	TMap<int32, FItem> ItemInfoMap;
	//보유 칭호
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory" )
	TSet<FName> AcquiredTitleSet;
   
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(int32 ItemID);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AcquireTitle(FName ItemID);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(int32 ItemID);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PrintInventory();
};
