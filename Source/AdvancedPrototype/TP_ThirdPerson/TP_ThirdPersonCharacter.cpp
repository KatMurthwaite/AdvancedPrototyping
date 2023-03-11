#include "TP_ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera boom"));
	arm->SetupAttachment(RootComponent);
	arm->TargetArmLength = 300.0f;
	arm->bUsePawnControlRotation = true;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(arm, USpringArmComponent::SocketName);
	camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	turnRate = 45;
	
	lookUpRate = 45;

	jumping = false;

	speed = 0.5f;
	
	walking = true;

	crouching = false;

	isClimbing = false;

	levels.Add("ThirdPersonMap");
	levels.Add("StarterMap");
	levels.Add("Minimal_Default");
}

void ATP_ThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (jumping)
	//{
	//	Jump();
	//}
}

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Horizontal", this, &ATP_ThirdPersonCharacter::HorizontalMove);
	InputComponent->BindAxis("Vertical", this, &ATP_ThirdPersonCharacter::VerticalMove);
	InputComponent->BindAxis("HorizontalRotation", this, &ATP_ThirdPersonCharacter::HorizontalRotation);
	InputComponent->BindAxis("VerticalRotation", this, &ATP_ThirdPersonCharacter::VerticalRotation);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ATP_ThirdPersonCharacter::CheckJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ATP_ThirdPersonCharacter::CheckJump);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ATP_ThirdPersonCharacter::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ATP_ThirdPersonCharacter::Sprint);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &ATP_ThirdPersonCharacter::CheckCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &ATP_ThirdPersonCharacter::CheckCrouch);

	PlayerInputComponent->BindAxis("ChangeLevel", this, &ATP_ThirdPersonCharacter::SwitchLevel);
}

void ATP_ThirdPersonCharacter::HorizontalMove(float value)
{
	if (Controller != NULL && value != 0.0)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator Yaw(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value * speed);
	}

	if (isClimbing)
	{
		AddMovementInput(FVector(0, 0, 0), value);
	}
}

void ATP_ThirdPersonCharacter::VerticalMove(float value)
{
	if (Controller != NULL && value != 0.0)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator Yaw(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value * speed);
	}

	if (isClimbing)
	{
		AddMovementInput(FVector(0, 0, 0), value);
	}
}

void ATP_ThirdPersonCharacter::HorizontalRotation(float value)
{
	AddControllerYawInput(value * GetWorld()->GetDeltaSeconds() * turnRate);
}

void ATP_ThirdPersonCharacter::CheckJump()
{
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	if (jumping)
	{
		jumping = false;
	}
	else
	{
		jumping = true;
		Jump();
	}
}

void ATP_ThirdPersonCharacter::VerticalRotation(float value)
{
	AddControllerPitchInput(value * GetWorld()->GetDeltaSeconds() * lookUpRate);
}

void ATP_ThirdPersonCharacter::Sprint()
{
	walking = !walking;

	if (walking)
	{
		speed = 0.5f;
	}
	else
	{
		speed = 1.0f;
	}
}

void ATP_ThirdPersonCharacter::CheckCrouch()
{
	if (crouching)
	{
		crouching = false;
		UnCrouch();
	}
	else
	{
		crouching = true;
		Crouch();
	}
}

void ATP_ThirdPersonCharacter::SwitchLevel(float direction)
{
	if (direction)
	{
		FString current = GetWorld()->GetMapName();
		current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		for (int i = 0; i < levels.Num(); i++)
		{
			if (current == levels[i])
			{
				if (i == 0 && direction < 0)
				{
					i = levels.Num();
				}
				else if (i == levels.Num() - 1 && direction > 0)
				{
					i = -1;
				}
				UGameplayStatics::OpenLevel(GetWorld(), FName(levels[i + direction]));
				break;
			}
		}
	}
}