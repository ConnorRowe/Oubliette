// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteStartScreen.h"

TArray<FSaveSlotStruct> UOublietteStartScreen::SortSaves(TArray<FSaveSlotStruct> SaveSlots, ESortMethodEnum SortMethod, bool isInverse)
{
	bool swapped = true;
	int length = SaveSlots.Num();

	//loops as long as a swap has occured in the last pass
	while (swapped)
	{
		swapped = false;

		for (int i = 1; i <= length - 1; ++i)
		{
			switch (SortMethod)
			{
			case ESortMethodEnum::ESME_Name:
			{
				//thankfully FName has a builtin compare function
				if (SaveSlots[i - 1].LineageName.Compare(SaveSlots[i].LineageName, ESearchCase::IgnoreCase) > 0)
					swapped = true;

				break;
			}
			case ESortMethodEnum::ESME_Level:
			{
				//if there is a smaller element found to the right then swap
				if (SaveSlots[i - 1].LineageLevel > SaveSlots[i].LineageLevel)
					swapped = true;

				break;
			}
			case ESortMethodEnum::ESME_DateTime:
			{
				if (SaveSlots[i - 1].DTLastPlayed > SaveSlots[i].DTLastPlayed)
					swapped = true;

				break;
			}
			}

			//If a swap should occur
			if (swapped)
			{
				SaveSlots.SwapMemory(i - 1, i);
			}
		}
	}

	//if it should be descending, reverse the array
	if (isInverse)
		Algo::Reverse(SaveSlots);

	return SaveSlots;
}