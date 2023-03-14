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
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void HorizontalMove(float value);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void VerticalMove(float value);
	void HorizontalRotation(float value);
	void VerticalRotation(float value);

	void CheckJump();

	void Sprint();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void CheckCrouch();

	bool jumping;
	
	UCameraComponent* camera;

	USpringArmComponent* arm;

	float speed;
	
	bool walking;

	bool crouching;

	float turnRate;
	
	float lookUpRate;
	
	bool isClimbing;
};

