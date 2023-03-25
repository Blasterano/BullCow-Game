// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    // Setup game
    SetupGame();    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    
    // if gameover then reset the game
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking guess
    {
        ProcessGuess(PlayerInput);        
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num()) - 1];
    Lives = HiddenWord.Len();
    bGameOver = false;

    //Welcoming our player 
    PrintLine(TEXT("Welcome to BULL COWS"));
    PrintLine(TEXT("Guess the %i letters word."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));     // promt to guess
    PrintLine(TEXT("The hidden word is: %s"), *HiddenWord); // debug line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("You Won!"));
        EndGame();
        return;
    }

    // check right no. of letters
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The word is %i letters long.\ntry again..."), HiddenWord.Len());
        return;
    }

    // check if an isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters. Try again."));
        return;
    }

    // remove life
    PrintLine(TEXT("You have lost a life."));
    --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You Lost!"));
        PrintLine(TEXT("The hidden word is: %s"), *HiddenWord);
        EndGame();
        return;        
    }

    // show bulls and cows
    PrintLine(TEXT("Sorry, try guessing again.\nGuesses left: %i"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len()-1; Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}