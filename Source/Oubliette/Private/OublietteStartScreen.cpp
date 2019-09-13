// Fill out your copyright notice in the Description page of Project Settings.


#include "OublietteStartScreen.h"

TArray<FLineageSaveStruct> UOublietteStartScreen::SortSaves(TArray<FLineageSaveStruct> SaveArray, ESortMethodEnum SortMethod, bool isInverse)
{
	bool swapped = true;
	int length = SaveArray.Num();

	//loops as long as a swap has occured in the last pass
	while (swapped)
	{
		swapped = false;

        for (int i = 1; i <= length-1; ++i)
        {
            switch (SortMethod)
            {
            case ESortMethodEnum::ESME_Name:
            {
				//thankfully FName has a builtin compare function
                if (SaveArray[i-1].FamilyName.Compare(SaveArray[i].FamilyName, ESearchCase::IgnoreCase) > 0)
                    swapped = true;

                break;
            }
            case ESortMethodEnum::ESME_Level:
            {
                //if there is a smaller element found to the right then swap
                if (SaveArray[i-1].Level > SaveArray[i].Level)
                    swapped = true;

                break;
            }
            case ESortMethodEnum::ESME_DateTime:
            {
                if (SaveArray[i-1].LastPlayed > SaveArray[i].LastPlayed)
                    swapped = true;

				break;
            }
            }

			//If a swap should occur
			if (swapped)
			{
				SaveArray.SwapMemory(i-1, i);
			}
        }
	}

	//if it should be descending, reverse the array
	if (isInverse)
		Algo::Reverse(SaveArray);

	return SaveArray;
}