#include "CSessionRow.h"
#include "Components/Button.h"
#include "CMainMenu.h"

void UCSessionRow::PostCreated(class UCMainMenu* InParentWidget, uint32 InIndex)
{
	ParentWidget = InParentWidget;
	SelfIndex = InIndex;

	RowButton->OnClicked.AddDynamic(this, &UCSessionRow::OnClicked);
}

void UCSessionRow::OnClicked()
{
	ParentWidget->SetSelectedSessionIndex(SelfIndex);
}

void UCSessionRow::SetClicked(bool InClicked)
{
	bClicked = InClicked;
}
