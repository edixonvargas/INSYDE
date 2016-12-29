#include "anntrainingdialog.h"

ANNTrainingDialog::ANNTrainingDialog(MultilayerPerceptron *mlp, TrainingSet *ts, QWidget *parent) :
	BasicDialog(parent)
{
	init(mlp, ts, NULL, NULL);
}

ANNTrainingDialog::ANNTrainingDialog(MultilayerPerceptron *mlp, TrainingSet *ts, TrainingSet *vs, TrainingSet *testset, QWidget *parent):
	BasicDialog(parent)
{
	init(mlp, ts, vs, testset);
}

ANNTrainingDialog::ANNTrainingDialog(MLPObject *gmlp, QWidget *parent) :
	BasicDialog(parent)
{
	init(gmlp->getMultilayerPerceptron(),
		 gmlp->getTrainingSet(),
		 gmlp->getValidationSet(),
		 gmlp->getTestSet());
}

ANNTrainingDialog::~ANNTrainingDialog()
{
	//TODO: 30/4/16 ~ANNTrainingDialog: implement
}

void ANNTrainingDialog::setUsingScrollArea(bool u)
{
	//TODO: must be implemented this and all functions regarding to fitting controlls to scroll area
	usingScrollArea = u;
}

bool ANNTrainingDialog::getUsingScrollArea() const
{
	return usingScrollArea;
}

void ANNTrainingDialog::setTrainingSet(TrainingSet *ts)
{
	if(trainingSet != ts){
		trainingSet = ts;
	}
}

TrainingSet *ANNTrainingDialog::getTrainingSet() const
{
	return trainingSet;
}

void ANNTrainingDialog::setValidationSet(TrainingSet *vs)
{
	if(validationSet != vs){
		validationSet = vs;
	}
}

TrainingSet *ANNTrainingDialog::getValidationSet() const
{
	return validationSet;
}

void ANNTrainingDialog::setTestSet(TrainingSet *ts)
{
	if (testSet != ts) {
		testSet = ts;
	}
}

TrainingSet *ANNTrainingDialog::getTestSet() const
{
	return testSet;
}

void ANNTrainingDialog::setArtificialNeuralNetwork(MultilayerPerceptron *mlp)
{
	this->mlp = mlp;
}

MultilayerPerceptron *ANNTrainingDialog::getMultilayerPerceptron() const
{
	return mlp;
}

void ANNTrainingDialog::saveClick()
{

}

void ANNTrainingDialog::setCanEditANNType(bool b)
{
	canEditANN = b;
}

bool ANNTrainingDialog::getCanEditANNType() const
{
	return canEditANN;
}

void ANNTrainingDialog::setCanEditInputSize(bool b)
{
	canEditInSize = b;
}

bool ANNTrainingDialog::getCanEditInputSize() const
{
	return canEditInSize;
}

void ANNTrainingDialog::setCanEditOutputSize(bool b)
{
	canEditOutSize = b;
}

bool ANNTrainingDialog::getCanEditOutputSize() const
{
	return canEditOutSize;
}

void ANNTrainingDialog::setEnableMultipleTraining(bool b)
{
	actionMultipleTraining->setEnabled(b);
}

bool ANNTrainingDialog::getEnableMultipleTraining() const
{
	return actionMultipleTraining->isEnabled();
}

void ANNTrainingDialog::closeEvent(QCloseEvent *)
{
	mlp->stopTraining();
}

void ANNTrainingDialog::onOuputSizeChanged()
{
	lisbOutputs->setValue(mlp->getOutputsSize());
}

void ANNTrainingDialog::onTrainingFinished(MLPTrainingResult tres)
{
	(void)tres;
	switch(currentTrainingStatus){
		case ANNTrainingDialog::Running:
			setTrainingStatus(Finished);
			break;
		case ANNTrainingDialog::Pausing:
			setTrainingStatus(Paused);
			break;
		case ANNTrainingDialog::Stopping:
			this->tres->clear();
			setTrainingStatus(Stopped);
			break;

		case ANNTrainingDialog::Paused:
		case ANNTrainingDialog::Stopped:
		case ANNTrainingDialog::Finished:
		default:
			break;
	}

}

void ANNTrainingDialog::multipleTrainingResult(MLPTrainingResult *tres)
{
	QString wnefp = multipleReportPath;
	wnefp.truncate(multipleReportPath.lastIndexOf('.'));

	QString ext =  multipleReportPath;
	ext = ext.mid(ext.lastIndexOf('.'));

	while(QFile::exists(wnefp + QString::number(fileIndex) + ext)){
		fileIndex++;
	}
	createFile(wnefp + QString::number(fileIndex) + ext, tres, tres->getMSESize());
}

void ANNTrainingDialog::onBtnEditTrainingSetClicked()
{
	trainingSetDialog = new TrainingSetDialog(trainingSet);

	if(trainingSetDialog->exec() == QDialog::Accepted){

		TrainingSet *ts;;
		if(isValidTrainingSet(ts = trainingSetDialog->getTrainingSet())){
			trainingSet = ts;
		}else{
			//TODO: implement
		}
	}
}

void ANNTrainingDialog::onTrainingAlgorithmChanged(int index)
{
	switch(index){
		case 0:
			ta = MultilayerPerceptron::Backpropagation;
			break;
		case 1:
			ta = MultilayerPerceptron::GradientDescendent;
			break;
		case 2:
			ta = MultilayerPerceptron::ScaledConjugateGradient;
			break;
	}
}

void ANNTrainingDialog::onTrasnferFunctionChanged(int index)
{
	switch(index){
		case 0:
			mlp->setTransferFunctionType(MultilayerPerceptron::Sigmoid);
			break;
		case 1:
			mlp->setTransferFunctionType(MultilayerPerceptron::Tanh);
			break;
	}
}

void ANNTrainingDialog::init(MultilayerPerceptron *imlp, TrainingSet *ts, TrainingSet *vs, TrainingSet *testS)
{
	QSpinBox
			*sbInValue,
			*sbOutValue;

	const int
			decimals = 5,
			max = 999999,
			min = -999999/*,
			step = 0.01*/;

	const double
			minError = 0.5;

	//Init bool variables
	usingScrollArea = false; //TOTO: Initialized but not used. Must be implemented
	canEditANN = true;
	canEditInSize = true;
	canEditOutSize = true;

	//Se inicializan todos los QVBoxLayout
	vlyMainScrollArea = new QVBoxLayout();
	vlyTrainingConditions = new QVBoxLayout();
	vlyToolButtons = new QVBoxLayout();
	vlyStopConditionBox = new QVBoxLayout();
	vlyOptimizationInternalLayout = new QVBoxLayout();
	vlyOptimizationWidget = new QVBoxLayout();

	//Initialize every QHBoxLayout
	lyInitialSettings = new QHBoxLayout();
	hlyStopConditionError = new QHBoxLayout();
	hlyLayers = new QHBoxLayout();
	hlyRandomize = new QHBoxLayout();

	//Initialize every QGridLayout
	vlyPlotter = new QVBoxLayout();

	//Se inicializan todos los QScrollArea
	scrollArea = new QScrollArea();
	saOptimization = new QScrollArea();

	//Initialize every QWidget
	scrollAreaContent = new QWidget();
	wOptimizations = new QWidget();

	//Initialize every QGroupBox
	gbProperties = new QGroupBox("Propiedades de la red");
	gbStopCondition = new QGroupBox(QString::fromLatin1("Condici�n de parada"));
	gbOptimization = new QGroupBox(QString::fromLatin1("Optimizaci�n"));
	gbPlotter = new QGroupBox("Resultados");

	//Inicilize every QFormLayout
	flyPropertiesBox = new QFormLayout();

	//Initialize every LabeledIntegerSpinBox
	lisbInputs = new LabeledIntegerSpinBox("Entradas");
	lisbOutputs = new LabeledIntegerSpinBox("Salidas");
	lisbEpochs = new LabeledIntegerSpinBox(QString::fromLatin1("�pocas"), 200000);

	//Initialize every LabeledComboBox
	lcbAlgorithm = new LabeledComboBox("Algoritmo");
	lcbTransferFunction = new LabeledComboBox(QString::fromLatin1("Funci�n de transferencia"));
	lcbANNType = new LabeledComboBox("Red Neuronal");

	//Initializa every LabeledDoubleSpinBoxWidget
	ldsbLearningRate = new LabeledDoubleSpinBox("Taza de aprendizaje", 0.5);
	ldsbSlope = new LabeledDoubleSpinBox("Pendiente", 1);
	ldsbThreshold = new LabeledDoubleSpinBox("Umbral", 0.5);
	ldsbRndFrom = new LabeledDoubleSpinBox("desde", -0.01);
	ldsbRndTo = new LabeledDoubleSpinBox("hasta", 0.01);
	ldsbMinError = new LabeledDoubleSpinBox("", minError);

	//Initialize every QPushButton
	btnEditTrainingSet = new QPushButton("Conjunto de entrenamiento...");
	btnEditValidationSet = new QPushButton(QString::fromLatin1("Conjunto de validaci�n..."));
	btnEditTestSet = new QPushButton("Conjunto de prueba...");

	//Initialize every QToolButton
	btnTraining = new QToolButton();
	btnAddLayer = new QToolButton();
	btnDeleteLayer = new QToolButton();
	btnWeights = new QToolButton();

	//Initialize every QSpacerItem
	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	//Initialize every QComboBox
	cbStopCondition = new QComboBox();

	//Initialize every QTableWidget
	tblLayers = new QTreeView();

	//Initialize every QCheckBox
	chkSA = new QCheckBox("Simulated Annealing");

	//Initialize every QLabel
	lblTime = new QLabel();

	//Initialize every QMenu
	mnuTraining = new QMenu();
	mnuWeights = new QMenu();

	//Initialize every QAction
	actionStartTraining = new QAction(ICON_TRAINING, "Entrenar", mnuTraining);
	actionStopTraining = new QAction(ICON_STOP, "Detener", mnuTraining);
	actionMultipleTraining = new QAction(ICON_MULTIPLE_TRAINING, "Programar entrenamiento...", mnuTraining);
	actionClearChart = new QAction(ICON_CLEAR, QString::fromLatin1("Limpiar gr�fica"), mnuTraining);

	//Initialize every SimulatedAnnealingWidget
	wSimulatedAnnealing = new SimulatedAnnealingWidget(0.01, 200, 50,  1, 0.9, 4, -0.02, 0.02);

	//Initialize every KDChart::Chart
	chart = new KDChart::Chart();

	//Initialize every KDChart::Plotter
	plotter = new KDChart::Plotter();

	//Initialize every KDChart::CartesianAxis
	xAxis = new KDChart::CartesianAxis(plotter);
	yAxis = new KDChart::CartesianAxis(plotter);

	//Initialize every KDChart::Legend
	legend = new KDChart::Legend(plotter, chart);

	//Initialize every WeightEditorDialog
	weightEditor = new WeightEditorDialog();

	//TODO: 26/4/16 implement all ANNs
	QStringList annTypeList;
	annTypeList <<
//				   "Adaline" <<
//				   "Hopfield" <<
				   "MLP" //<<
//				   "Simple Perceptron" <<
//				   "Kohonen"
				   ;

	lcbANNType->getComboBox()->addItems(annTypeList);

	//==================================================================================================================
	sbInValue = lisbInputs->getIntegerSpinBox();
	sbInValue->setReadOnly(!canEditInSize);
	if(canEditInSize){
		sbInValue->setButtonSymbols(QSpinBox::PlusMinus);
	}else{
		sbInValue->setButtonSymbols(QSpinBox::NoButtons);
	}

	//==================================================================================================================
	sbOutValue = lisbOutputs->getIntegerSpinBox();
	sbOutValue->setReadOnly(!canEditOutSize);
	if(canEditOutSize){
		sbOutValue->setButtonSymbols(QSpinBox::PlusMinus);
	}else{
		sbOutValue->setButtonSymbols(QSpinBox::NoButtons);
	}

	//==================================================================================================================
	tblLayers->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
	tblLayers->setAlternatingRowColors(true);
	tblLayers->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

	//==================================================================================================================
	btnAddLayer->setIcon(ICON_PLUS);

	//==================================================================================================================
	btnDeleteLayer->setIcon(ICON_MINUS);

	//==================================================================================================================
	vlyToolButtons->addWidget(btnAddLayer);
	vlyToolButtons->addWidget(btnDeleteLayer);
	vlyToolButtons->addItem(verticalSpacer);

	//==================================================================================================================
	hlyLayers->addWidget(tblLayers);
	hlyLayers->addLayout(vlyToolButtons);

	//==================================================================================================================
	lcbAlgorithm->getComboBox()->addItem("Backpropagation");

	//==================================================================================================================
	lcbTransferFunction->getComboBox()->addItem("Sigmoide");
	lcbTransferFunction->getComboBox()->addItem("Tangente");

	//==================================================================================================================
	ldsbSlope->setDecimals(decimals);
//	ldsbSlope->getDoubleSpinBox()->setSingleStep(step);

	//==================================================================================================================
	ldsbLearningRate->setDecimals(decimals);
//	ldsbLearningRate->getDoubleSpinBox()->setSingleStep(step);

	//==================================================================================================================
	cbStopCondition->addItem(QString::fromLatin1("MSE m�nimo"));
	cbStopCondition->addItem(QString::fromLatin1("RMSE m�nimo"));
	cbStopCondition->addItem(QString::fromLatin1("CE m�nimo"));

	//==================================================================================================================
	ldsbMinError->setLabelVisible(false);
	ldsbMinError->setDecimals(decimals);
	ldsbMinError->setMaximumValue(max);
	ldsbMinError->setValue(minError);

	//==================================================================================================================
	hlyStopConditionError->addWidget(cbStopCondition);
	hlyStopConditionError->addWidget(ldsbMinError);

	//==================================================================================================================
	ldsbThreshold->setDecimals(decimals);

	//==================================================================================================================
	ldsbRndFrom->setMinimumValue(min);
	ldsbRndFrom->setMaximumValue(max);
	ldsbRndFrom->setDecimals(decimals);

	//==================================================================================================================
	ldsbRndTo->setMinimumValue(min);
	ldsbRndTo->setMaximumValue(max);
	ldsbRndTo->setDecimals(decimals);

	//==================================================================================================================
	actionRandomizeWeights = mnuWeights->addAction(ICON_DICE, "Aleatorizar pesos");
	actionEditWeights = mnuWeights->addAction(ICON_EDIT, "Editar pesos...");

	//==================================================================================================================
	btnWeights->setDefaultAction(actionRandomizeWeights);
	btnWeights->setPopupMode(QToolButton::MenuButtonPopup);
	btnWeights->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	btnWeights->setText(actionRandomizeWeights->text());
	btnWeights->setMenu(mnuWeights);

	//==================================================================================================================
	hlyRandomize->addWidget(btnWeights);
	hlyRandomize->addWidget(ldsbRndFrom);
	hlyRandomize->addWidget(ldsbRndTo);

	//==================================================================================================================
	vlyStopConditionBox->addLayout(hlyStopConditionError);
	vlyStopConditionBox->addWidget(lisbEpochs);
	vlyStopConditionBox->addWidget(ldsbThreshold);
	vlyStopConditionBox->addLayout(hlyRandomize);

	//==================================================================================================================
	gbStopCondition->setLayout(vlyStopConditionBox);

	//==================================================================================================================
	vlyOptimizationWidget->setAlignment(Qt::AlignTop);
	vlyOptimizationWidget->addWidget(chkSA);
	vlyOptimizationWidget->addWidget(wSimulatedAnnealing);

	//==================================================================================================================
	wOptimizations->setLayout(vlyOptimizationWidget);

	//==================================================================================================================
//	saOptimization->setWidgetResizable(true);
	saOptimization->setWidget(wOptimizations);

	//==================================================================================================================
	vlyOptimizationInternalLayout->addWidget(saOptimization);

	//==================================================================================================================
	gbOptimization->setLayout(vlyOptimizationInternalLayout);

	//==================================================================================================================
	vlyTrainingConditions->addWidget(gbStopCondition);
	vlyTrainingConditions->addWidget(gbOptimization);

	//==================================================================================================================
	xAxis->setPosition(KDChart::CartesianAxis::Bottom);
//	xAxis->setAlignment(Qt::AlignJustify);
	xAxis->setTitleText("Epocas");

	//==================================================================================================================
	yAxis->setPosition(KDChart::CartesianAxis::Left);
	yAxis->setTitleText("Error");

	//==================================================================================================================

	legend->setPosition(KDChart::Position::East);
	legend->setAlignment(Qt::AlignCenter);
	legend->setTitleText(tr("Leyenda"));
	legend->setOrientation(Qt::Vertical);
	legend->setShowLines(false);
	legend->setVisible(true);

	//==================================================================================================================
	plotter->addAxis(xAxis);
	plotter->addAxis(yAxis);

	//==================================================================================================================
	chart->addLegend(legend);
	chart->coordinatePlane()->replaceDiagram(plotter);
	chart->coordinatePlane()->setRubberBandZoomingEnabled(true);

	//==================================================================================================================
	vlyPlotter->addWidget(chart);
	vlyPlotter->addWidget(lblTime);

	//==================================================================================================================
	gbPlotter->setLayout(vlyPlotter);

	//==================================================================================================================
	actionStopTraining->setEnabled(false);

	actionClearChart->setEnabled(false);

	//==================================================================================================================
	mnuTraining->addAction(actionStartTraining);
	mnuTraining->addAction(actionStopTraining);
	mnuTraining->addAction(actionClearChart);
	mnuTraining->addAction(actionMultipleTraining);
	mnuTraining->setDefaultAction(actionStartTraining);

	//==================================================================================================================
	btnTraining->setPopupMode(QToolButton::MenuButtonPopup);
	btnTraining->setDefaultAction(actionStartTraining);
	btnTraining->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	btnTraining->setMenu(mnuTraining);

	//==================================================================================================================
	flyPropertiesBox->addRow(lcbANNType);
	flyPropertiesBox->addRow(lisbInputs);
	flyPropertiesBox->addRow(lisbOutputs);
	flyPropertiesBox->addRow(hlyLayers);
	flyPropertiesBox->addRow(lcbAlgorithm);
	flyPropertiesBox->addRow(lcbTransferFunction);
	flyPropertiesBox->addRow(ldsbSlope);
	flyPropertiesBox->addRow(ldsbLearningRate);
	flyPropertiesBox->addRow(btnEditTrainingSet);
	flyPropertiesBox->addRow(btnEditValidationSet);
	flyPropertiesBox->addRow(btnEditTestSet);

	//==================================================================================================================
	gbProperties->setLayout(flyPropertiesBox);

	//==================================================================================================================
	lyInitialSettings->addWidget(gbProperties);
	lyInitialSettings->addLayout(vlyTrainingConditions);

	//==================================================================================================================
	vlyMainScrollArea->addLayout(lyInitialSettings);
	vlyMainScrollArea->addWidget(gbPlotter);

	//==================================================================================================================
	scrollAreaContent->setLayout(vlyMainScrollArea);

	//==================================================================================================================
	scrollArea->setWidget(scrollAreaContent);
	scrollArea->setWidgetResizable(true);

	//==================================================================================================================
	setCanEditInputSize(true);
	setCanEditOutputSize(true);

	//==================================================================================================================
	setEnableSave(false);
	setEnableSaveAs(false);
	setEnableOpen(false);
	setEnableMultipleTraining(false);
	setHelpMenuVisible(false);
	setHelpButtonVisible(false);
	setToolsMenuVisible(false);
	setViewMenuVisible(false);
	setEnablePreferences(false);
	setEnableNew(false);
	setEnableExport(false);
	setEnableImport(false);
	setEnableUnDoFramework(false);
	setEnableClipboardFramework(false);
	getRejectButton()->setText("Cerrar");
	setAcceptButtonVisible(false);
	getPreferencesAction()->setVisible(false);

	//Must be inserted after index 1 because 0 is Stretch Item
	insertButton(1, btnTraining);

	getMainWindow()->setCentralWidget(scrollArea);
	setWindowModality(Qt::NonModal);
	setWindowFlags(Qt::Window);

	//Se asigna el perceptron
	setArtificialNeuralNetwork(imlp);

	mlp->setSaveTrainingResults(true);
	mlp->setEnabledElapsedTimeEvent(true);
	mlp->setDefaultRandomRange(ldsbRndFrom->getValue(), ldsbRndTo->getValue());

	tres = mlp->getTrainingResult();
	plotter->setModel(tres);

	setTrainingSet(ts);
	setValidationSet(vs);
	setTestSet(testS);

	setTrainingStatus(Stopped);

	ta = MultilayerPerceptron::Backpropagation;

	lisbInputs->setValue(mlp->getInputsSize());
	lisbOutputs->setValue(mlp->getOutputsSize());

	annModel = new ANNModelWrapper(mlp);

	annModel->setCanEditLayerSize(true);
	annModel->setVisibleNeurons(false);
	annModel->setVisibleWeightValues(true);
	annModel->setVisibleOutputLayer(true);
	annModel->setViewType(ANNModelWrapper::Table);

	tblLayers->setModel(annModel);
	tblLayers->resizeColumnToContents(0);

	btnDeleteLayer->setEnabled(mlp->getHiddenLayerCount() > 1);
	lcbAlgorithm->getComboBox()->setCurrentIndex(0);

	onCbStopConditionCurrentIndexChanged(0);
	onBtnRandomizeClicked();
	wSimulatedAnnealing->setVisible(false);


	//Connect save action
	connect(getFileMenu()->actions()[1], SIGNAL(triggered()), SLOT(saveClick()));
	connect(actionClearChart, SIGNAL(triggered()), SLOT(onClearChartClicked()));
	connect(lcbTransferFunction->getComboBox(), SIGNAL(currentIndexChanged(int)), SLOT(onTrasnferFunctionChanged(int)));
	connect(lcbAlgorithm->getComboBox(), SIGNAL(currentIndexChanged(int)), SLOT(onTrainingAlgorithmChanged(int)));
	connect(actionStartTraining, SIGNAL(triggered()), SLOT(onTrainClicked()));
	connect(actionStopTraining, SIGNAL(triggered()), SLOT(onStopTrainingClicked()));
	connect(actionMultipleTraining, SIGNAL(triggered()), SLOT(onMultipleTrainingClicked()));
	connect(chkSA, SIGNAL(toggled(bool)), SLOT(onChkSimAnnToggled(bool)));
	connect(actionRandomizeWeights, SIGNAL(triggered()), SLOT(onBtnRandomizeClicked()));
	connect(actionEditWeights, SIGNAL(triggered()), SLOT(onEditWeightsClicked()));
	connect(cbStopCondition, SIGNAL(currentIndexChanged(int)), SLOT(onCbStopConditionCurrentIndexChanged(int)));
	connect(btnEditTrainingSet, SIGNAL(clicked()), SLOT(onBtnEditTrainingSetClicked()));
	connect(btnEditValidationSet, SIGNAL(clicked()), SLOT(onBtnEditValidationTestClicked()));
	connect(btnEditTestSet, SIGNAL(clicked()), SLOT(onBtnEditTestSetClicked()));
	connect(btnAddLayer, SIGNAL(clicked()), SLOT(onBtnAddLayerClicked()));
	connect(btnDeleteLayer, SIGNAL(clicked()), SLOT(onDeleteLayerClicked()));
	connect(mlp, SIGNAL(elapsedTime(MLPTrainingResult)), SLOT(updateStatusLabels(MLPTrainingResult)));
	connect(mlp, SIGNAL(trainingFinished(MLPTrainingResult)), SLOT(onTrainingFinished(MLPTrainingResult)));
	connect(mlp, SIGNAL(outputSizeChanged()), SLOT(onOuputSizeChanged()));
	connect(lisbInputs->getIntegerSpinBox(), SIGNAL(valueChanged(int)), SLOT(onANNInputChanged(int)));
	connect(lisbOutputs->getIntegerSpinBox(), SIGNAL(valueChanged(int)), SLOT(onANNOutputChanged(int)));
	connect(lcbANNType->getComboBox(), SIGNAL(currentIndexChanged(int)), SLOT(onAnnTypeSelectionChanged(int)));
	connect(ldsbRndFrom->getDoubleSpinBox(), SIGNAL(valueChanged(double)), SLOT(onRndFromValueChanged(double)));
	connect(ldsbRndTo->getDoubleSpinBox(), SIGNAL(valueChanged(double)), SLOT(onRndToValueChanged(double)));
}

void ANNTrainingDialog::createFile(QString path, MLPTrainingResult *tr, int nsamples){
	QFile *f = new QFile(path);
	if (f->open(QFile::ReadWrite)) { // file opened successfully
		f->write("", qstrlen(""));
		QTextStream t( f ); // use a text stream
		QString s("");
		const QString sep = "\t";
		//		int sWeights;
		int inc = tr->getEpochs() / nsamples < 1 ? 1 : tr->getEpochs() / nsamples;
		for(unsigned int i = 0; i <= tr->getEpochs(); i += inc){
			s += QString::number(i) + sep + QString::number(tr->getMSEHistory()[i]) + "\n\r";
		}
		t << s;
		f->close();
	}else if(path != ""){
		QString msg;
		switch(f->error()){
			case QFile::OpenError:
				msg = "No se pudo guardar el archivo.\nPosiblemente el archivo esta abierto o no existe.";
				break;
			case QFile::NoError:
			case QFile::ReadError:
			case QFile::WriteError:
			case QFile::FatalError:
			case QFile::ResourceError:
			case QFile::AbortError:
			case QFile::TimeOutError:
			case QFile::UnspecifiedError:
			case QFile::RemoveError:
			case QFile::RenameError:
			case QFile::PositionError:
			case QFile::ResizeError:
			case QFile::PermissionsError:
			case QFile::CopyError:
				msg = "Ocurrio un error inesperado al intentar guardar el archivo.\nPor favor intentelo de nuevo.";
				break;
		}
		QMessageBox::critical(this, "Error", msg);
	}
	delete f;
}

void ANNTrainingDialog::disableAllControls()
{
	chkSA->setEnabled(false);
	cbStopCondition->setEnabled(false);
	lcbAlgorithm->setEnabled(false);
	lcbTransferFunction->setEnabled(false);
	tblLayers->setEnabled(false);
	btnAddLayer->setEnabled(false);
	btnDeleteLayer->setEnabled(false);
	btnEditTestSet->setEnabled(false);
	btnEditTrainingSet->setEnabled(false);
	btnEditValidationSet->setEnabled(false);
	btnWeights->setEnabled(false);
	ldsbMinError->setEnabled(false);
	lisbInputs->setEnabled(false);
	lisbOutputs->setEnabled(false);

	lisbEpochs->setEnabled(false);
	ldsbLearningRate->setEnabled(false);
	ldsbRndFrom->setEnabled(false);
	ldsbRndTo->setEnabled(false);
	ldsbSlope->setEnabled(false);

	wSimulatedAnnealing->setEnabled(false);
}

void ANNTrainingDialog::setBPEnabledControls(bool enabled)
{
	cbStopCondition->setEnabled(enabled);
	chkSA->setEnabled(enabled);
	lcbAlgorithm->setEnabled(enabled);
	lcbTransferFunction->setEnabled(enabled);
	lisbInputs->setEnabled(enabled);
	lisbOutputs->setEnabled(enabled);
	tblLayers->setEnabled(enabled);

	btnAddLayer->setEnabled(enabled);
	btnDeleteLayer->setEnabled(enabled);
	btnEditTestSet->setEnabled(enabled);
	btnEditTrainingSet->setEnabled(enabled);
	btnEditValidationSet->setEnabled(enabled);
	btnWeights->setEnabled(enabled);

	lisbEpochs->setEnabled(enabled);
	ldsbLearningRate->setEnabled(enabled);
	if(enabled){
		onCbStopConditionCurrentIndexChanged(cbStopCondition->currentIndex());
	}else{
		ldsbMinError->setEnabled(enabled);
		//		ui->sbMinErrorClasification->setEnabled(enabled);
		//		ui->sbMinRMSError->setEnabled(enabled);
	}
	ldsbRndFrom->setEnabled(enabled);
	ldsbRndTo->setEnabled(enabled);
	ldsbSlope->setEnabled(enabled);
}

void ANNTrainingDialog::updateSAControls()
{
	wSimulatedAnnealing->setVisible(chkSA->isChecked());
	wSimulatedAnnealing->setEnabled(chkSA->isChecked());
}

void ANNTrainingDialog::updateActionClearChart()
{
	if(tres->rowCount() > 0 && !mlp->isTraining()){
		actionClearChart->setEnabled(true);
	}else{
		actionClearChart->setEnabled(false);
	}

}

bool ANNTrainingDialog::isValidTrainingSet(TrainingSet *ts)
{
	//FIXME: 30/4/16 isValidTrainingSet: not working, check it
	if(ts->getInputsSize() == mlp->getInputsSize() &&
	   ts->getTargetsSize() == mlp->getOutputsSize()){
		return true;
	}

	return false;
}

void ANNTrainingDialog::setTrainingStatus(ANNTrainingDialog::TrainingStatus ts)
{
	switch(ts){
		case ANNTrainingDialog::Finished:
			updateActionClearChart();
			updateStatusLabels(*tres);
			setBPEnabledControls(true);
			updateSAControls();

			actionStartTraining->setIcon(ICON_TRAINING);
			actionStartTraining->setText("Entrenar");
			actionStopTraining->setEnabled(false);
			btnTraining->setText(actionStartTraining->text());

			currentTrainingStatus = Finished;

			raise();
			break;
		case ANNTrainingDialog::Running:{
			disableAllControls();

			actionStartTraining->setIcon(ICON_PAUSE);
			actionStartTraining->setText("Pausar");
			actionStopTraining->setEnabled(true);
			btnTraining->setText(actionStartTraining->text());
			actionClearChart->setEnabled(false);

			BackpropagationSettings *bp = new BackpropagationSettings(
											  lisbEpochs->getValue(),
											  (StopCondition)cbStopCondition->currentIndex(),
											  ldsbMinError->getValue(),
											  ldsbLearningRate->getValue());

			SimulatedAnnealingSettings *sa = new SimulatedAnnealingSettings(
												 wSimulatedAnnealing->getMinTemperature(),
												 wSimulatedAnnealing->getSamples(),
												 wSimulatedAnnealing->getNumberOfChanges(),
												 wSimulatedAnnealing->getStartCondition(),
												 wSimulatedAnnealing->getStartTemperature(),
												 wSimulatedAnnealing->getMinNoise(),
												 wSimulatedAnnealing->getMaxNoise(),
												 wSimulatedAnnealing->getDecTempFactor());
			//Si esta habilitado Simulated Annealing
			if(chkSA->isChecked()){
				mlp->startTraining(trainingSet, bp, sa);
			}else{
				mlp->startTraining(trainingSet, bp);
			}

			currentTrainingStatus = Running;
			break;
		}
		case ANNTrainingDialog::Pausing:
			mlp->stopTraining();
			actionStartTraining->setIcon(ICON_WAITING);
			actionStartTraining->setText("Pausando...");
			actionStopTraining->setEnabled(false);
			btnTraining->setText(actionStartTraining->text());
			actionClearChart->setEnabled(false);

			currentTrainingStatus = Pausing;
			break;
		case ANNTrainingDialog::Paused:
			updateActionClearChart();
			updateStatusLabels(*tres);
			setBPEnabledControls(true);
			updateSAControls();

			actionStartTraining->setIcon(ICON_TRAINING);
			actionStartTraining->setText("Continuar");
			actionStopTraining->setEnabled(true);
			btnTraining->setText(actionStartTraining->text());

			currentTrainingStatus = Paused;
			break;
		case ANNTrainingDialog::Stopping:
			mlp->stopTraining();

			actionStartTraining->setIcon(ICON_WAITING);
			actionStartTraining->setText("Deteniendo...");
			actionStopTraining->setEnabled(false);
			btnTraining->setText(actionStartTraining->text());
			actionClearChart->setEnabled(false);

			currentTrainingStatus = Stopping;
			break;
		case ANNTrainingDialog::Stopped:
			updateActionClearChart();
			updateStatusLabels(*tres);
			setBPEnabledControls(true);
			updateSAControls();

			actionStartTraining->setIcon(ICON_TRAINING);
			actionStartTraining->setText("Entrenar");
			actionStopTraining->setEnabled(false);
			btnTraining->setText(actionStartTraining->text());

			raise();

			currentTrainingStatus = Stopped;
			break;

		default:
			break;
	}
}

void ANNTrainingDialog::updateControls(ArtificialNeuralNetwork *ann)
{
	switch(ann->getType()){
		case ann_base::ArtificialNeuralNetwork::NoType:
			break;
		case ann_base::ArtificialNeuralNetwork::Adaline:
			break;
		case ann_base::ArtificialNeuralNetwork::SimplePerceptron:
			break;
		case ann_base::ArtificialNeuralNetwork::MultilayerPerceptron:
			break;
		case ann_base::ArtificialNeuralNetwork::Hopfiel:
			break;
		case ann_base::ArtificialNeuralNetwork::Kohonen:
			break;

	}
}

void ANNTrainingDialog::onTrainClicked()
{
	//Start validation process
	QMessageBox message;
	QStringList strings;
	bool incongruence = false;

	switch(currentTrainingStatus){
		case ANNTrainingDialog::Running:
			setTrainingStatus(Pausing);
			break;
		case ANNTrainingDialog::Pausing:
		case ANNTrainingDialog::Stopping:
			break;
		case ANNTrainingDialog::Finished:
		case ANNTrainingDialog::Paused:
		case ANNTrainingDialog::Stopped:
		default:

			if(mlp->getInputsSize() != trainingSet->getInputsSize()){
				strings << QString::fromLatin1("- El n�mero de entradas de la red neuronal no coincide con el n�mero de entradas suministradas por el conjunto de entrenamiento. \n\n");
				incongruence = true;
			}
			if(mlp->getOutputsSize() != trainingSet->getTargetsSize()){
				strings << QString::fromLatin1("- El n�mero de salidas de la red neuronal no coincide con el n�mero de salidas suministradas por el conjunto de entrenamiento. \n\n");
				incongruence = true;
			}

			if(incongruence){
				QString wholeString;

				wholeString = QString::fromLatin1("Se han detectado los siguientes errores:\n\n");

				for(int i = 0; i < strings.size(); i++){
					wholeString += strings[i];
				}

				wholeString += QString::fromLatin1("Debe corregir estos errores para poder continuar.");

				message.setWindowTitle("Se detectaron irregularidades");
				message.setIcon(QMessageBox::Warning);
				message.setText(wholeString);

				message.exec();
				break;
			}

			setTrainingStatus(Running);
			break;

	}
}

void ANNTrainingDialog::onStopTrainingClicked()
{
	switch(currentTrainingStatus){
		case ANNTrainingDialog::Running:
			setTrainingStatus(Stopping);
			break;
		case ANNTrainingDialog::Paused:
			tres->clear();
			setTrainingStatus(Stopped);
			break;
		case ANNTrainingDialog::Finished:
		case ANNTrainingDialog::Pausing:
		case ANNTrainingDialog::Stopping:
		case ANNTrainingDialog::Stopped:
			break;

	}
}

void ANNTrainingDialog::onClearChartClicked()
{
	if(tres->rowCount() != 0){
		tres->clear();
		actionClearChart->setEnabled(false);
	}
}

void ANNTrainingDialog::onCancelClicked()
{
	//WARNING: onCancelClicked must be validated to finish training before close
	close();
}

void ANNTrainingDialog::onBtnRandomizeClicked()
{
	mlp->randomizeWeights(ldsbRndFrom->getValue(), ldsbRndTo->getValue());
}

void ANNTrainingDialog::onEditWeightsClicked()
{
	weightEditor = new WeightEditorDialog(mlp, this);

	weightEditor->setViewMenuVisible(false);

	weightEditor->setWindowFlags(windowFlags() | Qt::Window);

	weightEditor->setWindowModality(Qt::NonModal);

	weightEditor->show();
}

void ANNTrainingDialog::updateStatusLabels(MLPTrainingResult tres)
{
	if(tres.isEmpty()){
		xAxis->setTitleText("Epocas: 0");

		lblTime->setText("Tiempo transcurrido: 0");
	}else{
		xAxis->setTitleText("Epocas: " + QString::number(tres.getEpochs()));

		lblTime->setText("Tiempo transcurrido: " + QString::number(tres.getTime()));
		//	lblTime->setText("Tiempo transcurrido: " + QTime(0, 0, tres.getTime()).toString("hh:mm:ss"));
	}
}

void ANNTrainingDialog::generateReport()
{
	//TODO: implement
}

void ANNTrainingDialog::onANNInputChanged(int value)
{
	mlp->setInputSize(value);
	mlp->randomizeWeights(ldsbRndFrom->getValue(), ldsbRndTo->getValue());
}

void ANNTrainingDialog::onANNOutputChanged(int value)
{
	mlp->setOutputSize(value);
	mlp->randomizeWeights(ldsbRndFrom->getValue(), ldsbRndTo->getValue());
}

void ANNTrainingDialog::onAnnTypeSelectionChanged(int index)
{
	switch(index){
		case 0: //"Adaline"
			updateControls(adaline);
			break;
		case 1: //"Hopfield"
			updateControls(hopfield);
			break;
		case 2: //"MLP"

			updateControls(mlp);

			break;
		case 3: //"Simple Perceptron"
			updateControls(sp);
			break;
		case 4: //"Kohonen"
			updateControls(kohonen);
			break;
		default:
			break;
	}
}

void ANNTrainingDialog::onRndFromValueChanged(double value)
{
	mlp->setDefaultRandomMinimum(value);
}

void ANNTrainingDialog::onRndToValueChanged(double value)
{
	mlp->setDefaultRandomMaximum(value);
}

void ANNTrainingDialog::exportData()
{
	if(!tres->isEmpty()){
		QString path = QFileDialog::getSaveFileName(
						   this,
						   "Ruta del archivo",
						   "resultado",
						   tr("Valores separados por comas (*.csv);;Archivo EXCEL (*.xls);;Archivo XML (*.xml)")
						   );
		if(path != ""){
			SamplesDialog sd(tres, this);
			if(sd.exec() == QDialog::Accepted){
				createFile(path, tres, sd.getSampleCount());
				//TODO: corregir boton de guardar
				//				saveFile->setEnabled(false);
				return;
			}
		}
	}else{
		QMessageBox::critical(this, "Error", "No hay ningun dato para guardar");
	}

	//TODO: corregir boton de guardar
	//	saveFile->setEnabled(true);
}

void ANNTrainingDialog::onBtnEditValidationTestClicked()
{
	if(validationSet != NULL){
		validationSetDialog = new TrainingSetDialog(validationSet);
	}else{
		validationSetDialog = new TrainingSetDialog(validationSet = new TrainingSet(mlp->getInputsSize(), mlp->getOutputsSize()));
	}
	if(validationSetDialog->exec() == QDialog::Accepted){
		validationSet = validationSetDialog->getTrainingSet();
	}
	delete validationSetDialog;
}

void ANNTrainingDialog::onBtnEditTestSetClicked()
{
	if(testSet != NULL){
		testSetDialog = new TrainingSetDialog(testSet);
	}else{
		testSetDialog = new TrainingSetDialog(testSet = new TrainingSet(mlp));
	}
	if(testSetDialog->exec() == QDialog::Accepted){
		testSet = testSetDialog->getTrainingSet();
	}
	delete testSet;
}

void ANNTrainingDialog::onBtnAddLayerClicked()
{
	bool ok = false;
	int val = QInputDialog::getInt(this,
								   "Numero de elementos",
								   "Neuronas",
								   10,
								   1,
								   999999,
								   1,
								   &ok);


	if(ok){

		mlp->addLayer(val);

		btnDeleteLayer->setEnabled(mlp->getHiddenLayerCount() > 1);
	}
}

void ANNTrainingDialog::onDeleteLayerClicked()
{
	QItemSelectionModel *selection = tblLayers->selectionModel();
	QModelIndexList selList = selection->selectedIndexes();

	if(!selList.isEmpty()){
		if(selList[0].row() != mlp->getHiddenLayerCount() + 1 && mlp->getHiddenLayerCount() > 1){
			mlp->removeLayer(selList[0].row());
			if(mlp->getHiddenLayerCount() > 1){
				btnDeleteLayer->setEnabled(true);
			}else{
				btnDeleteLayer->setEnabled(false);
			}
		}
	}
}

void ANNTrainingDialog::onMultipleTrainingClicked()
{
	bool ok = false;

	multipleReportPath = QFileDialog::getSaveFileName(
							 this,
							 "Ruta del archivo",
							 "resultado",
							 tr("Valores separados por comas (*.csv);;Archivo EXCEL (*.xls);;Archivo XML (*.xml)"),
							 0,
							 QFileDialog::ShowDirsOnly);
	if(multipleReportPath == ""){
		return;
	}

	int nSamples = QInputDialog::getInt(this,
										"Numero de entrenamientos",
										"Numero",
										1,
										1,
										1000000,
										1,
										&ok);
	fileIndex = 0;
	if(ok){
		QVector<MultilayerPerceptron*> arr(nSamples);
		//		TrainingSet *trainingSet = gmlp->getTrainingSet();

		BackpropagationSettings *bp = new BackpropagationSettings(
										  lisbEpochs->getValue(),
										  (StopCondition)cbStopCondition->currentIndex(),
										  ldsbMinError->getValue(),
										  //											  ui->sbMinRMSError->value(),
										  //											  ui->sbMinErrorClasification->value(),
										  ldsbLearningRate->getValue());


		SimulatedAnnealingSettings *sa = wSimulatedAnnealing->getSimulatedAnnealingSettings();

		for(int i = 0; i < nSamples; i++){
			arr[i] = new MultilayerPerceptron(mlp->getInputsSize(),
											  mlp->getOutputsSize(),
											  mlp->getLayerSizes(),
											  mlp->getTransferFunctionType());

			arr[i]->randomizeWeights(ldsbRndFrom->getValue(), ldsbRndTo->getValue());

			connect(arr[i], SIGNAL(onTrainingFinished(MLPTrainingResult)), SLOT(multipleTrainingResult(MLPTrainingResult)));

			//Si esta habilitado Simulated Annealing
			if(chkSA->isChecked()){
				arr[i]->startTraining(trainingSet, bp, sa);
			}else{
				arr[i]->startTraining(trainingSet, bp);
			}
		}
	}
}

void ANNTrainingDialog::onCbStopConditionCurrentIndexChanged(int index)
{

	ldsbMinError->setEnabled(true);
	switch(index){
		case 0: //MSE
			ldsbThreshold->setEnabled(false);
			tres->setMSEVisible(true);
			tres->setRMSEVisible(false);
			tres->setCEVisible(false);
			break;
		case 1: //RMSE
			ldsbThreshold->setEnabled(false);
			tres->setMSEVisible(false);
			tres->setRMSEVisible(true);
			tres->setCEVisible(false);
			break;
		case 2: //CE
			ldsbThreshold->setEnabled(true);
			tres->setMSEVisible(false);
			tres->setRMSEVisible(false);
			tres->setCEVisible(true);
			break;
	}
}

void ANNTrainingDialog::onChkSimAnnToggled(bool checked)
{
	wSimulatedAnnealing->setVisible(checked);
	wSimulatedAnnealing->setEnabled(checked);
}
