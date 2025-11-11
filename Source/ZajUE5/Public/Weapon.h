#pragma once



#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UCLASS()
class ZAJUE5_API AWeapon : public AItem
{
	GENERATED_BODY()


protected:
	virtual void Interact_Implementation(AActor* OuterActor) override;
	

	void AttachToSocket(USceneComponent* InParent, const FName& InSocketName);
	void Equip(AActor* OuterActor);
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Weapon Properties")
	FName MainSocketName = FName("hand_rSocket");
};