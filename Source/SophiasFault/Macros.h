#pragma once

#define printText(_text) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, FString::Printf(TEXT(_text)))
#define printString(_string) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, _string)
#define printFName(_text) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, _text.ToString())
#define printFloat(_float) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("%f"), _float))
#define printInt(_int) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("%d"), _int))
#define printBool(_bool) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("%s"), _bool ? TEXT("TRUE") : TEXT("FALSE")))
#define printVector(_vector) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%s"), *_vector.ToString()))