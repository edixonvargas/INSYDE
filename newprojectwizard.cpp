#include "newprojectwizard.h"
#include "ui_newprojectwizard.h"

NewProjectWizard::NewProjectWizard(QWidget *parent) :
    QWizard(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    setPage(SelectProjectID, new SelectProjectPage());
    setPage(ChooseProjectNameID, new ChooseProjectNamePage());
    setPage(PerceptronPageID, new PerceptronPage());
    setPage(AdalinePageID, new AdalinePage());

    setStartId(SelectProjectID);

//    setSubTitleFormat(Qt::RichText);
//    setTitleFormat(Qt::RichText);

    setPixmap(QWizard::WatermarkPixmap, QPixmap(":imagenes/artificial-intelligence.png"));

#ifndef Q_WS_MAC
    setWizardStyle(ModernStyle);
#endif

    setOption(HaveHelpButton, true);
}
