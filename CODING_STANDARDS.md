# Game Development Coding Standards

## Overview
This document defines coding standards for game development projects using Unreal Engine (C++), Unity (C#), and Azure backend services. These standards ensure code quality, maintainability, and team collaboration.

---

## General Principles

### Code Quality
- Write self-documenting code with clear intent
- Follow DRY (Don't Repeat Yourself) principles
- Keep functions small and focused (single responsibility)
- Prefer composition over inheritance
- Write code that's easy to test and debug

### Version Control
- Use meaningful commit messages (present tense: "Add feature" not "Added feature")
- Keep commits atomic and focused
- Never commit commented-out code or TODO comments without a ticket reference
- Include ticket/issue numbers in commit messages

### Documentation
- Document public APIs and complex algorithms
- Keep documentation close to code (inline comments for "why", not "what")
- Maintain up-to-date README files for each module

---

## Unreal Engine C++ Standards

### Naming Conventions

#### Classes and Structs
```cpp
// Classes: Prefix with project abbreviation or 'U'/'A'/'F'
class AMyPlayerCharacter : public ACharacter { };  // Actors use 'A'
class UMyHealthComponent : public UActorComponent { };  // UObjects use 'U'
struct FPlayerStats { };  // Structs use 'F'
```

#### Variables
```cpp
// Member variables: Prefix based on type
bool bIsAlive;           // Booleans use 'b'
int32 CurrentHealth;     // PascalCase for members
float MovementSpeed;
TArray<AActor*> Enemies; // Collections are plural

// Local variables: Use descriptive camelCase
int32 damageAmount = 10;
float deltaTime = GetWorld()->GetDeltaSeconds();
```

#### Functions
```cpp
// Use verb + noun, PascalCase
void ApplyDamage(float DamageAmount);
bool IsPlayerAlive() const;
float CalculateDistance(const FVector& Target) const;

// Getters/Setters
int32 GetHealth() const { return CurrentHealth; }
void SetHealth(int32 NewHealth) { CurrentHealth = NewHealth; }
```

#### Constants and Enums
```cpp
// Constants: ALL_CAPS with underscores
const float MAX_HEALTH = 100.0f;
const int32 INVENTORY_SIZE = 20;

// Enums: Prefix with 'E', values PascalCase
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    None,
    Pistol,
    Rifle,
    Shotgun
};
```

### Code Organization

#### Header Files (.h)
```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

// Forward declarations
class UStaticMeshComponent;
class UMaterialInterface;

UCLASS()
class PROJECTNAME_API AMyActor : public AActor
{
    GENERATED_BODY()

public:
    // Constructor
    AMyActor();

    // Public functions
    virtual void Tick(float DeltaTime) override;

protected:
    // Protected functions
    virtual void BeginPlay() override;

private:
    // Private functions
    void InternalUpdate();

    // UPROPERTY variables (organized by access level)
    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
    float Health;
};
```

#### Implementation Files (.cpp)
```cpp
#include "MyActor.h"
#include "Components/StaticMeshComponent.h"

AMyActor::AMyActor()
{
    PrimaryActorTick.bCanEverTick = true;
    
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    // Implementation
}
```

### Best Practices

#### Memory Management
```cpp
// Use smart pointers for non-UObject classes
TSharedPtr<FMyData> DataPtr = MakeShared<FMyData>();
TWeakPtr<FMyData> WeakDataPtr = DataPtr;

// Never manually delete UObjects - use MarkPendingKill() or let GC handle it
// Use UPROPERTY() to prevent garbage collection

// Prefer const references for large parameters
void ProcessData(const FLargeStruct& Data);
```

#### Error Handling
```cpp
// Use check() for critical errors (crashes in development)
check(IsValid(PlayerController));

// Use ensure() for recoverable errors (logs but continues)
if (!ensure(Health > 0))
{
    Health = 0;
    return;
}

// Log errors appropriately
UE_LOG(LogTemp, Error, TEXT("Failed to load asset: %s"), *AssetPath);
```

#### Performance
```cpp
// Cache frequently accessed components in BeginPlay
void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();
    CachedMeshComponent = GetMesh();
}

// Use const correctness
float CalculateDamage() const;

// Avoid GetComponentByClass in Tick - cache references
// Use object pooling for frequently spawned actors
```
