// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAIBaseCharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAIBaseCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

public:
    ASTUAIBaseCharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* UBehaviorTreeAsset;

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* HealthWidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    float HealthVisibilityDistance = 1000.0f;

    virtual void BeginPlay() override;

    virtual void OnDeath() override;
    virtual void OnHealthChanged(float Health, float HealthDelta) override;

private:
    void UpdateHealthWidgetVisibility();
};
