// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "TP_ThirdPersonCharacter.generated.h"

UCLASS(config = Game)
class ATP_ThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATP_ThirdPersonCharacter();

	void SwitchLevel(float direction);

	TArray<FString> levels;

protected:
	virtual void Tick(float DeltaTime) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:
	void HorizontalMove(float value);
	void VerticalMove(float value);
	void HorizontalRotation(float value);
	void VerticalRotation(float value);

	void CheckJump();

	void Sprint();

	UPROPERTY()
		bool jumping;

	UPROPERTY()
		UCameraComponent* camera;

	UPROPERTY()
		USpringArmComponent* arm;

	UPROPERTY()
		float speed;
	
	UPROPERTY()
		bool walking;
};

