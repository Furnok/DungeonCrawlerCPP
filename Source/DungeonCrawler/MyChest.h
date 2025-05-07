// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyChest.generated.h"

UCLASS()
class DUNGEONCRAWLER_API AMyChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyChest();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* ChestMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    class UBoxComponent* TriggerBox;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void OpenChest();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chest")
    int Money = 20.f;

private:
    bool bPlayerInRange = false;
    bool bIsOpen = false;
};
