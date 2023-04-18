주석이 달린 C++ 코드는 Unreal Engine 4용 게임 개발에 사용되는 코드입니다. 코드의 각 부분에 대해 자세히 설명드리겠습니다.

#pragma once: 이 지시문은 해당 헤더 파일이 한 번만 포함되도록 합니다. 이는 컴파일러에게 헤더 파일이 중복으로 포함되지 않도록 지시하는 역할을 합니다.

#include "CoreMinimal.h": Unreal Engine 4의 기본 헤더 파일을 포함합니다. 이 헤더 파일에는 엔진에서 사용하는 기본 데이터 형식, 매크로, 유틸리티 등이 포함되어 있습니다.

#include "GameFramework/Actor.h": Actor 클래스의 헤더 파일을 포함합니다. Actor는 Unreal Engine 4에서 모든 게임 오브젝트의 기본 클래스입니다.

#include "MyActor.generated.h": Unreal Engine 4에서 생성하는 코드를 포함하는 헤더 파일입니다. 이 파일은 Unreal Engine 4의 리플렉션 시스템, 네이티브 클래스와 오브젝트를 생성하는 데 필요한 코드를 포함합니다.

UCLASS(): 이 매크로는 AMyActor 클래스를 Unreal Engine 4의 UObject 시스템과 연결합니다. 이를 통해 클래스의 메타데이터를 엔진에 전달할 수 있습니다.

class TESTUNREALENGINE_API AMyActor : public AActor: AMyActor 클래스를 선언하고 AActor 클래스를 상속합니다. TESTUNREALENGINE_API 매크로는 이 클래스가 다른 모듈에서 사용될 수 있도록 노출시킵니다.

GENERATED_BODY(): Unreal Engine 4가 생성하는 코드와 클래스를 결합하는 매크로입니다. 클래스 내에서 이 매크로를 사용하여 클래스에 대한 리플렉션 데이터를 생성합니다.

AMyActor();: AMyActor 클래스의 생성자를 선언합니다. 생성자는 클래스의 인스턴스가 생성될 때 호출되며, 클래스의 기본 속성을 설정하는 데 사용됩니다.

virtual void BeginPlay() override;: AActor 클래스의 BeginPlay 함수를 오버라이드합니다. BeginPlay 함수는 게임이 시작되거나 액터가 스폰될 때 호출되며, 게임 로직 초기화에 사용됩니다.

virtual void Tick(float DeltaTime) override;: AActor 클래스의 Tick 함수를 오버라이드합니다. Tick 함수는 매 프레임 호출되며, 게임 로직 업데이트에 사용됩니다.

UStaticMeshComponent* Mesh;: UStaticMeshComponent 포인터를 선언합니다. 이 컴포넌트는 3D 모델을 렌더링하고 액터와 상호 작용하는 데 사용됩니다. 이 변수는 MyActor 클래스의 인스턴스에 대한 포인터를 저장합니다.

UPROPERTY(VisibleAnywhere): 이 매크로는 액터의 속성에 대한 메타데이터를 설정합니다. 여기서는 Mesh 속성이 편집기에서 어디에서나 볼 수 있도록 설정합니다.

UPROPERTY(EditAnywhere, Category=BattleStat): 이 매크로는 편집기에서 Hp 및 Mp 속성을 수정할 수 있도록 설정하고, "BattleStat" 카테고리에 속성을 분류합니다.

int32 Hp;: 정수형 변수 Hp를 선언합니다. 이 변수는 캐릭터의 체력(Health Point)를 나타냅니다.

int32 Mp;: 정수형 변수 Mp를 선언합니다. 이 변수는 캐릭터의 마나 포인트(Mana Point)를 나타냅니다.

UPROPERTY(EditAnywhere, Category = BattleStat): 이 매크로는 RotateSpeed 속성을 편집기에서 수정할 수 있도록 설정하고, "BattleStat" 카테고리에 속성을 분류합니다.

float RotateSpeed = 30.f;: 부동소수점 변수 RotateSpeed를 선언하고 초기값으로 30.0f를 할당합니다. 이 변수는 캐릭터가 회전하는 속도를 나타냅니다.

이 코드는 AMyActor라는 클래스를 선언하고, 이 클래스의 인스턴스가 게임 환경에서 어떻게 동작하는지를 정의합니다. AMyActor는 AActor 클래스를 상속받아 기본 게임 오브젝트의 동작을 확장하고, 커스텀 속성과 로직을 추가할 수 있습니다.