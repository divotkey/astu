#include "CryptBlockComponent.h"
#include "cryptlib.h"
#include "blowfish.h"

// Sets default values for this component's properties
CryptBlockComponent::CryptBlockComponent()
	: CurRotation(0, 0, 0)
	, PitchSpeed(0)
	, YawSpeed(0)
	, RollSpeed(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	// 
	UE_LOG(LogTemp, Warning, TEXT("CryptBlock component constructor called"));
}


void CryptBlockComponent::InitializeComponent()
{
	Super::InitializeComponent();
	//UE_LOG(LogTemp, Warning, TEXT("CryptBlock component initialized"));
	updatedComponent = GetOwner()->GetRootComponent();
	if (!updatedComponent) {
		UE_LOG(LogTemp, Warning, TEXT("CryptBlock component didn't find root component"));
	}
}

// Called when the game starts
void CryptBlockComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CryptBlock component received BeginPlay"));
}

// Called every frame
void CryptBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurRotation.Pitch += PitchSpeed * DeltaTime;
	CurRotation.Yaw += YawSpeed * DeltaTime;
	CurRotation.Roll += RollSpeed * DeltaTime;
	updatedComponent->SetWorldRotation(CurRotation, false);
}