#include "InventoryWidget.h"
#include "../Sophia.h"
#include "../Macros.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "InventoryComponent.h"
#include "Items/Item.h"
#include "Items/ItemPhysic.h"
#include "Kismet/KismetMathLibrary.h" 

// https://dev.epicgames.com/community/learning/tutorials/KJ9e/unreal-engine-arranging-widgets-in-a-circle
void UInventoryWidget::UpdateInventory()
{
    _playerCharacter = Cast<ASophia>(GetOwningPlayerPawn());
    if (_playerCharacter)
        _itemsWidgetClass = _playerCharacter->GetItemWidgetClass();
    else
        _itemsWidgetClass = nullptr;

    if (UCanvasPanel* allItemsPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("AllItems")))) {
        allItemsPanel->ClearChildren();

        if (_playerCharacter) {
            FVector initialRotationAxis = FVector(0, -1, 0).RotateAngleAxis(0, FVector(0, 0, 1));
            float initialAngleAxis = 360.f / _playerCharacter->GetInventory()->_items.Num();

            for (int i = 0; i < _playerCharacter->GetInventory()->_items.Num(); i++) {
                if (_playerCharacter->GetInventory()->_items[i] != nullptr) {
                    float angleAxis = initialAngleAxis * i;
                    FVector rotateInitialRotation = initialRotationAxis.RotateAngleAxis(angleAxis, FVector(0, 0, 1));

                    rotateInitialRotation *= 350.f; // Radius
                    FVector2D initialPosition = FVector2D(rotateInitialRotation.X, rotateInitialRotation.Y);

                    if (_itemsWidgetClass) {
                        if (UUserWidget* itemWidget = CreateWidget<UUserWidget>(GetWorld(), _itemsWidgetClass)) {
                            if (itemWidget) {
                                Cast<UTextBlock>(itemWidget->GetWidgetFromName(TEXT("ItemName")))->SetText(FText::FromString(_playerCharacter->GetInventory()->_items[i]->_name));
                                UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(allItemsPanel->AddChild(itemWidget));
                                if (canvasSlot) {
                                    canvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
                                    canvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
                                    canvasSlot->SetPosition(initialPosition);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Make the items widget rotate around the center of the inventory widget
//FRotator baseWidgetRotation = UKismetMathLibrary::MakeRotFromX(rotateInitialRotation);
//baseWidgetRotation.Yaw += 90.f;
//SetRenderTransformAngle(baseWidgetRotation.Yaw);