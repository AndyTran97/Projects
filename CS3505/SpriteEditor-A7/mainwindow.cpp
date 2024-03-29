#include "mainwindow.h"
#include "helpmenu.h"
#include "ui_mainwindow.h"
#include "model.h"
#include "canvasform.h"
#include "frameview.h"

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(model)
    , helpMenu(this)
{
    ui->setupUi(this);


    // [=== INITIALIZE SECTION ===]
    updatePaintColor(DEFAULT_PAINT_COLOR);// Tzhou: initialize the current color label to black
    setColorScenesToViews();// Tzhou: Initialize the views in color picker area.
    setIconToToolBtns(); // Jeffrey: is there a way we can move this to only mainwindow.ui?

    //Canvas
    ui->canvasView->updatePixmap(&model.canvas);
    frameList.push_back(model.canvas);

    //Preview
    previewScene->addItem(&imageItem);
    previewScene->setFocusItem(&imageItem);
    ui->graphicsView->setScene(previewScene);
    initializePreview();
    isInit = false;

    //Frame View
    initializeFrameView();


    // [=== CANVAS CONNECTIONS ===] @Jeffrey
    connect(&model, &Model::updateCanvas, this, &MainWindow::updateCanvas);
    connect(&model, &Model::updatePreviewCanvas, this, &MainWindow::updatePreviewCanvas);

    // --- Canvas Input ---
    connect(ui->canvasView, &ImageViewEditor::mouseDown, &model, &Model::mouseDown);
    connect(ui->canvasView, &ImageViewEditor::mouseMove, &model, &Model::mouseMove);
    connect(ui->canvasView, &ImageViewEditor::mouseRelease, &model, &Model::mouseRelease);

    // [=== PREVIEW CONNECTIONS ===] @Andy Tran
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    connect(ui->fpsSlider, &QSlider::valueChanged, this, &MainWindow::onChangeFpsSliderValue);
    connect(this, &MainWindow::changeFPS, &model, &Model::changeFPS);

    // [=== TOOL CONNECTIONS ===] @Ruini
    // --- Tool Select ---
    connect(ui->pencilBtn,&QPushButton::clicked,this,[=](){
        emit changeTool(PENCIL);
    });
    connect(ui->pickerBtn,&QPushButton::clicked,this,[=](){
        emit changeTool(PICKER);
    });
    connect(ui->eraserBtn,&QPushButton::clicked,this,[=](){
        emit changeTool(ERASER);
    });
    connect(ui->bucketBtn,&QPushButton::clicked,this,[=](){
        emit changeTool(BUCKET);
    });

    // --- Tool Box ---
    connect(this, &MainWindow::changeTool, &model, &Model::changeTool);
    connect(this, &MainWindow::pencilCursor, ui->canvasView, &ImageViewEditor::pencilCursor);
    connect(this, &MainWindow::eraserCursor, ui->canvasView, &ImageViewEditor::eraserCursor);
    connect(this, &MainWindow::pickerCursor, ui->canvasView, &ImageViewEditor::pickerCursor);
    connect(this, &MainWindow::bucketCursor, ui->canvasView, &ImageViewEditor::bucketCursor);
    connect(this, &MainWindow::changeTool, this, &MainWindow::disableTool);

    // --- Tool Settings ---
    connect(ui->toolSlider, &QSlider::valueChanged, &model, &Model::setPenSize);
    connect(ui->toolSlider, &QSlider::valueChanged,this,&MainWindow::changeSizeSliderValue);


    // [=== COLOR CONNECTIONS ===] @TZHou @Ruini
    // --- Color Dialog ---
    connect(ui->changeColorBtn, &QPushButton::pressed, this, &MainWindow::changeColorBtnIsPressed);

    //Tzhou: Extra feature
    connect(ui->customColorView1, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(ui->customColorView2, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(ui->customColorView3, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(ui->customColorView4, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(ui->customColorView5, &DragAndDropGraphicsView::itemDrop,
            this, &MainWindow::imageEnter);
    connect(this, &MainWindow::updateCustomColor,
            &model, &Model::updateCustomColor);
    connect(ui->customColorView1, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);
    connect(ui->customColorView2, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);
    connect(ui->customColorView3, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);
    connect(ui->customColorView4, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);
    connect(ui->customColorView5, &DragAndDropGraphicsView::customColorIsSelected,
            &model, &Model::customColorIsSelected);


    // --- Color Model-View Updates ---
    //1. user pick a color => model's paintColor change.
    connect(this, &MainWindow::paintColorChanged, &model, &Model::paintColorChanged);
    //2. Model informs View to change color
    connect(&model, &Model::updatePaintColor, this, &MainWindow::updatePaintColor);

    // --- Color Alpha Channels ---
    connect(ui->alphaSlider, &QSlider::valueChanged, &model, &Model::updateColorRelated);
    connect(&model, &Model::updateAlphaSliderLabel, ui->alphaValueLabel, &QLabel::setText);
    connect(&model, &Model::resetAlphaSlider, ui->alphaSlider, &QSlider::setValue );

    // [=== MENU CONNECTIONS ===] @Duong
    connect(ui->actionNew_Project, &QAction::triggered, this, &MainWindow::handleNewCanvas);
    connect(ui->actionSave_Project, &QAction::triggered, this, &MainWindow::handleSaveCanvas);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::handleOpenCanvas);
    // @AndyTran
    connect(ui->actionSprite_Size, &QAction::triggered, this, &MainWindow::handleSize);
    // @Tzhou
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::displayAbout);
    //@Ruini Tong
    connect(ui->actionToolBox, &QAction::triggered, this, &MainWindow::displayHelpMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// [=== INITIALIZE SECTION ===] @Andy Tran
/*Code reviewed by Tingting Zhou*/
void MainWindow::initializePreview() {
    //Initialize - Preview
    imageItem.setPixmap(QPixmap::fromImage(frameList.front()));
    ui->graphicsView->fitInView(QRectF(0, 0, frameList.front().width(), frameList.front().height()),
                                Qt::KeepAspectRatio);
    ui->graphicsView->setSceneRect(imageItem.boundingRect());
    ui->graphicsView->centerOn(imageItem.boundingRect().center());
    timer->start(frameDuration);
}

/*Code reviewed by Tingting Zhou*/
void MainWindow::initializeFrameView(){
    previousFrame = 0;
    currentFrame = 0;

    QPushButton* addFrameBtn = new QPushButton("+");
    addFrameBtn->setFixedSize(42, 42);
    addFrameBtn->setStyleSheet("border: 1px solid grey;");

    QFont font = addFrameBtn->font();
    font.setPointSize(16);
    addFrameBtn->setFont(font);

    //Frames Layout
    framesHorizontalLayout->setAlignment(Qt::AlignLeft);
    framesHorizontalLayout->addWidget(addFrameBtn);
    ui->framesContents->setLayout(framesHorizontalLayout);

    //Add Frame Button Connection
    connect(addFrameBtn, &QPushButton::clicked, &model, &Model::onAddFrame);
    connect(addFrameBtn, &QPushButton::clicked, this, [=]() {
        addFrameWidget();
        framesHorizontalLayout->itemAt(frameList.size())->widget()->setStyleSheet("border: 1px solid grey;");
    });

    addFrameWidget();
}

// [=== FRAME VIEW SECTION ===] @Andy Tran
/*Code reviewed by Andy Duong*/
void MainWindow::addFrameWidget()
{
    //Andy Tran - need to fix the scroll bar issue
    QImage* currentImage = &model.canvas;

    //Not Initializing -> set currentImage according to the frameList
    if(!isInit){
        currentImage = &frameList[currentFrame];
    }

    FrameView* newFrame = new FrameView();

    //If the size is greater than 0 -> insert at index size - 1
    if(frameList.size() > 0){
        framesHorizontalLayout->insertWidget(currentFrame, newFrame);

        //Set the border style for the previous and current widgets
        framesHorizontalLayout->itemAt(previousFrame)->widget()->setStyleSheet("border: 1px solid grey;");
        framesHorizontalLayout->itemAt(currentFrame)->widget()->setStyleSheet("border: 3px solid black;");
    }
    //Initialize Frame View - insert at index 0
    else{
        framesHorizontalLayout->insertWidget(0, newFrame);
        framesHorizontalLayout->itemAt(0)->widget()->setStyleSheet("border: 3px solid black;");
    }


    newFrame->fitInView(QRectF(0, 0, currentImage->width(), currentImage->height()),
                        Qt::KeepAspectRatio);
    newFrame->updatePixmap(currentImage);

    // Set focus on the newest FrameView
    newFrame->setFocus();

    //Connect events of FrameView and Model
    connect(newFrame, &FrameView::mouseClicked, &model, &Model::mouseClicked);
    connect(newFrame, &FrameView::deletePressed, &model, &Model::deletePressed);
}

/*Code reviewed by Jeffery Le*/
void MainWindow::deleteFrameWidget(int deletedIndex) {
    //Delete a Widget at deletedIndex
    QLayoutItem *item = framesHorizontalLayout->takeAt(deletedIndex);
    delete item->widget();
    delete item;
}

/*Code reviewed by Ruini Tong*/
void MainWindow:: deleteAllWidgets(){
    // Clear the current frame widgets
    while (!framesHorizontalLayout->isEmpty()) {
        QLayoutItem* item = framesHorizontalLayout->takeAt(0);
        delete item->widget();
        delete item;
    }
}

/*Code reviewed by Jeffery Le*/
void MainWindow::loadFrameWidgets() {

    //Andy Tran: Need to optimized

    // Clear the current frame widgets
    deleteAllWidgets();

    //Add Push Button
    QPushButton* addFrameBtn = new QPushButton("+");
    addFrameBtn->setFixedSize(42, 42);
    addFrameBtn->setStyleSheet("border: 1px solid grey;");

    QFont font = addFrameBtn->font();
    font.setPointSize(16);
    addFrameBtn->setFont(font);

    //Frames Layout
    framesHorizontalLayout->setAlignment(Qt::AlignLeft);
    framesHorizontalLayout->addWidget(addFrameBtn);

    //Add Frame Button Connection
    connect(addFrameBtn, &QPushButton::clicked, &model, &Model::onAddFrame);
    connect(addFrameBtn, &QPushButton::clicked, this, [=]() {
        addFrameWidget();
        framesHorizontalLayout->itemAt(frameList.size())->widget()->setStyleSheet("border: 1px solid grey;");
    });

    // Create frame widgets for each frame in the frameList
    for (size_t i = 0; i < frameList.size(); ++i) {
        previousFrame = currentFrame;
        currentFrame = i;
        addFrameWidget();
    }

    //Set the border of last frame to nonn
    framesHorizontalLayout->itemAt(currentFrame)->widget()->setStyleSheet("border: 1px solid grey;");

    //Set the selecting frame to the first one
    framesHorizontalLayout->itemAt(0)->widget()->setStyleSheet("border: 3px solid black;");

    //Set the current frame to the first one
    currentFrame = 0;
}


// [=== CANVAS SECTION ===] @Jeffrey @Andy Tran
/*Code reviewed by Andy Duong*/
void MainWindow::updateCanvas(QImage* canvas, vector<QImage>* list, int currentFrame, Action action, int newSize, int deletedIndex) {
    //Update canvas View
    ui->canvasView->fitInView(QRectF(0, 0, canvas->width(), canvas->height()), Qt::KeepAspectRatio);
    ui->canvasView->updatePixmap(canvas);

    //Update previous frame, current frame, action, frameList, canvas size
    this->canvasSize = newSize;
    this->action = action;
    previousFrame = currentFrame;

    if(action == UPDATE)
        previousFrame = this->currentFrame;

    this->currentFrame = currentFrame;
    frameList.clear();
    frameList.resize(list->size());
    frameList = *list;

    if(this->fps == 0){
        if(timer->isActive()){
            timer->stop();
         }
        imageItem.setPixmap(QPixmap::fromImage(frameList[currentFrame]));
        previewScene->setFocusItem(&imageItem);
        ui->graphicsView->setSceneRect(imageItem.boundingRect());
        ui->graphicsView->centerOn(imageItem.boundingRect().center());
    }
    else{
        if(timer->isActive()){
            timer->stop();
         }
        timer->start(frameDuration);
    }


    //Update Frame View Widgets and Preview
    FrameView *frame;
    switch(action){
    case DELETE_FRAME:
        if(deletedIndex < 0)
            break;

        //Delete widget at index
        deleteFrameWidget(deletedIndex);

        //Update color of border according to the currcent frame view
        framesHorizontalLayout->itemAt(this->currentFrame)->widget()->setStyleSheet("border: 3px solid black;");

        //Shift the index of FrameView elements to Left - Need to exclude the Add Frame Button (last element of the layout)
        for(int i = deletedIndex; i < framesHorizontalLayout->count() - 1; i++){
            FrameView *frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(i)->widget());
            if((frame->getIndex() - 1) >= 0)
                frame->updateIndex(frame->getIndex()-1);
        }

        // Set focus on the current FrameView
        frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(this->currentFrame)->widget());
        if(frame) frame->setFocus();
        break;
    case UPDATE:
        //Update color of border according to the currcent frame view
        framesHorizontalLayout->itemAt(previousFrame)->widget()->setStyleSheet("border: 1px solid grey;");
        framesHorizontalLayout->itemAt(this->currentFrame)->widget()->setStyleSheet("border: 3px solid black;");

        //Update the FrameView when modified Canvas
        frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(this->currentFrame)->widget());
        if(frame){
            frame->updatePixmap(&frameList[currentFrame]);

            // Set focus on the current FrameView
            frame->setFocus();
        }
        break;
    case OPEN_FILE:
        //To start initializing the preview
        loadFrameWidgets();

        // Reinitialize the preview components
        initializePreview();
        break;
    case CREATE_NEW:
        // Clear the current frame widgets
        deleteAllWidgets();

        // Reinitialize the frame view and preview components
        initializePreview();
        initializeFrameView();
        break;
    case RESIZE:
        //Resize all the images in the frame view
        for(int i = 0; i < framesHorizontalLayout->count() - 1; i++){
            FrameView *frame = qobject_cast<FrameView*>(framesHorizontalLayout->itemAt(i)->widget());
            if(frame){
                frame->fitInView(QRectF(0, 0, frameList[i].width(), frameList[i].height()), Qt::KeepAspectRatio);
                frame->updatePixmap(&frameList[i]);
            }
        }
        // Reinitialize the preview components
        initializePreview();
        break;
    default:
        break;
    }

}

// [=== PREVIEW SECTION ===] @Andy Tran
/*Code reviewed by Ruini Tong*/
void MainWindow::onTimerTimeout() {
    //Move to the next frame when QTimer timeout
    curPreviewIndex = (curPreviewIndex + 1) % frameList.size();
    imageItem.setPixmap(QPixmap::fromImage(frameList[curPreviewIndex]));
    previewScene->setFocusItem(&imageItem);
    ui->graphicsView->setSceneRect(imageItem.boundingRect());
    ui->graphicsView->centerOn(imageItem.boundingRect().center());
}

/*Code reviewed by Ruini Tong*/
void MainWindow::onChangeFpsSliderValue(int value)
{
    //Change FPS label
    QString textValue = QString::number(value);
    ui->fpsValueLabel->setText(textValue);
    fps = value;

    if(fps == 0){
        frameDuration = INT_MAX;
    }
    else{
        frameDuration = 1000 / fps;
    }
    emit changeFPS();
}

/**
 * @author Jeffrey Le
 * @brief MainWindow::updatePreviewCanvas
 * @param canvas
 */
/*Code reviewed by Ruini Tong*/
void MainWindow::updatePreviewCanvas(QImage* canvas) {
    ui->canvasView->updatePreviewPixmap(canvas);
}

// [=== TOOL SECTION ===] @Ruini
/*Code reviewed by Andy Duong*/
void MainWindow::disableTool(Tool tool){
    //enable all tools
    ui->pencilBtn->setEnabled(true);
    ui->pickerBtn->setEnabled(true);
    ui->eraserBtn->setEnabled(true);
    ui->bucketBtn->setEnabled(true);

    //disable selected tool
    switch (tool) {
    case PENCIL:
        ui->pencilBtn->setEnabled(false);
        emit pencilCursor();
        break;
    case PICKER:
        ui->pickerBtn->setEnabled(false);
        emit pickerCursor();
        break;
    case ERASER:
        ui->eraserBtn->setEnabled(false);
        emit eraserCursor();
        break;
    case BUCKET:
        ui->bucketBtn->setEnabled(false);
        emit bucketCursor();
        break;
    default:
        break;
    }
}

/*Code reviewed by Jeffery Le*/
void MainWindow::changeSizeSliderValue(int value){
    QString textValue = QString::number(value);
    ui->sizeValueLabel->setText(textValue);
}



// [=== COLOR SECTION ===] @TZHou @Ruini
/*Code reviewed by Jeffery Le*/
void MainWindow::updatePaintColor(QColor newColor)
{
    int h = ui->currentColorLabel->height();
    int w = ui->currentColorLabel->width();
    QPixmap pix(w, h);
    pix.fill( newColor);
    QPainter paint(&pix);
    ui->currentColorLabel->setPixmap(pix);
    ui->currentColorLabel->setParent(ui->colorPanel);
}

/*Code reviewed by Andy Tran*/
void MainWindow::changeColorBtnIsPressed()
{
    QColor color = QColorDialog::getColor(DEFAULT_PAINT_COLOR,this);
    if(color.isValid()) //If the "OK" button is pressed.
    {
        emit paintColorChanged(color); //Ruini Tong
    }
}

/*Code reviewed by Jeffery Le*/
void MainWindow::setIconToToolBtns(){
    QSize toolSize = QSize(30,30);
    ui->pencilBtn->setIcon(QIcon(QPixmap(":/images/icons/Pencil.PNG")));
    ui->pencilBtn->setIconSize(toolSize);
    ui->eraserBtn->setIcon(QIcon(QPixmap(":/images/icons/Eraser.PNG")));
    ui->eraserBtn->setIconSize(toolSize);
    ui->pickerBtn->setIcon(QIcon(QPixmap(":/images/icons/Picker.PNG")));
    ui->pickerBtn->setIconSize(toolSize);
    ui->bucketBtn->setIcon(QIcon(QPixmap(":/images/icons/Bucket.PNG")));
    ui->bucketBtn->setIconSize(toolSize);
    //Ruini add
    ui->changeColorBtn->setIcon(QIcon(QPixmap(":/images/icons/Color.PNG")));
    ui->changeColorBtn->setIconSize(QSize(100,100));

}

/*Code reviewed by Andy Tran*/
void MainWindow::setColorScenesToViews()
{
    ui->customColorView1->setScene(customeColorScene1);
    ui->customColorView2->setScene(customeColorScene2);
    ui->customColorView3->setScene(customeColorScene3);
    ui->customColorView4->setScene(customeColorScene4);
    ui->customColorView5->setScene(customeColorScene5);
}

/*Code reviewed by Andy Duong*/
void MainWindow::imageEnter(QPixmap pixmap, QGraphicsView *view)
{
    QGraphicsScene* scene = view->scene();
    if(scene->items().count()>0){
        scene->clear();
    }
    scene->addPixmap(pixmap.scaled(26,26));
    view->setScene(scene);
    view->viewport()->update();
    view->update();
    view->show();

    emit updateCustomColor(view);
}

// [=== MENU SECTION ===] @Duong
/*Code reviewed by Tingting Zhou*/
void MainWindow::handleNewCanvas() {
    CanvasForm form(this, canvasSize, CREATE_NEW);
    connect(&form, &CanvasForm::createNewCanvas, &model, &Model::createNewCanvas);
    form.exec();
}

/*Code reviewed by Tingting Zhou*/
void MainWindow::handleSaveCanvas()
{
    QString filter = "Sprite Sheet Project (*.ssp)";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Sprite Sheet Project"), "", filter, &filter);

    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".ssp")) {
            fileName += ".ssp";
        }
        model.saveFile(fileName);
    }
}

/*Code reviewed by Tingting Zhou*/
void MainWindow::handleOpenCanvas()
{
    QString filter = "Sprite Sheet Project (*.ssp)";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Sprite Sheet Project"), "", filter, &filter);

    if (!fileName.isEmpty()) {
        model.openFile(fileName);
    }
}

/*Code reviewed by Tingting Zhou*/
void MainWindow::handleSize() {
    CanvasForm form(this, canvasSize, RESIZE);
    connect(&form, &CanvasForm::resizeFrameList, &model, &Model::resizeFrameList);
    form.exec();
}

/*Code reviewed by Jeffery Le*/
void MainWindow::displayAbout()
{
    QMessageBox::information(
        this,
        tr("Sprite Editor"),
        tr(
           "Course: CS3505 2023 Spring\n"
           "Assignment: Sprite Editor\n"
           "Authors:\n"
           "\tAndy Duong\n"
           "\tAndy Tran\n"
           "\tJeffery Le\n"
           "\tRuini Tong\n"
           "\tTingting Zhou"));

}

/*Code reviewed by Jeffery Le*/
void MainWindow::displayHelpMenu()
{
    helpMenu.show();
}


