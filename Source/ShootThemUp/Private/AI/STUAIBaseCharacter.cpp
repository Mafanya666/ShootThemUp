// Shoot Them Up Game, All Rights Reserved

#include "AI/STUAIBaseCharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/STUHealthBarWidget.h"
#include "Components/STUHealthComponent.h"

ASTUAIBaseCharacter::ASTUAIBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAIBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthWidgetComponent);
}

void ASTUAIBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateHealthWidgetVisibility();
}

void ASTUAIBaseCharacter::OnDeath()
{
    Super::OnDeath();

    const auto STUController = Cast<AAIController>(Controller);
    if (STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}

void ASTUAIBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    Super::OnHealthChanged(Health, HealthDelta);

    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthBarWidget) return;
    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ASTUAIBaseCharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld() ||                              //
        !GetWorld()->GetFirstPlayerController() ||  //
        !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
        return;
    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}