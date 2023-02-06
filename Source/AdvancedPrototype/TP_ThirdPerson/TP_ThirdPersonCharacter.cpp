// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	arm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	arm->TargetArmLength = 300.f;
	arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	arm->bEnableCameraLag = true;
	arm->CameraLagSpeed = 2;
	arm->CameraLagMaxDistance = 1.5f;

	arm->bEnableCameraRotationLag = true;
	arm->CameraRotationLagSpeed = 4;
	arm->CameraLagMaxTimeStep = 1;

	camera->AttachToComponent(arm, FAttachmentTransformRules::KeepRelativeTransform);

	jumping = false;

	speed = 0.5f;
	
	walking = true;

	levels.Add("ThirdPersonMap");
	levels.Add("StarterMap");
	levels.Add("Minimal_Default");
}

void ATP_ThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (jumping)
	{
		Jump();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

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

	PlayerInputComponent->BindAxis("ChangeLevel", this, &ATP_ThirdPersonCharacter::SwitchLevel);
}

void ATP_ThirdPersonCharacter::HorizontalMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorRightVector(), value * speed);
	}
}

void ATP_ThirdPersonCharacter::VerticalMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorForwardVector(), value * speed);
	}
}

void ATP_ThirdPersonCharacter::HorizontalRotation(float value)
{
	if (value)
	{
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}

void ATP_ThirdPersonCharacter::CheckJump()
{
	GetCharacterMovement()->JumpZVelocity = 600.f;

	if (jumping)
	{
		jumping = false;
	}
	else
	{
		jumping = true;
	}
}

void ATP_ThirdPersonCharacter::VerticalRotation(float value)
{
	if (value)
	{
		float temp = arm->GetRelativeRotation().Pitch + value;
		if (temp < 25 && temp > -65)
		{
			arm->AddLocalRotation(FRotator(value, 0, 0));
		}
	}
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
		speed = 1;
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