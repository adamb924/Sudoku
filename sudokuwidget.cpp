#include <QtGui>

#include "sudokuwidget.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

SudokuWidget::SudokuWidget(QWidget *parent)
    : QWidget(parent)
{
    clearAll();

    setFocusPolicy(Qt::StrongFocus);

    margin = 10;

    cBackground.setRgb(255,255,255);
    cCellBackground.setRgb(225,225,225);
    cMinorGrid.setRgb(62,157,255);
    cMajorGrid.setRgb(0,0,255);
    cGivenEntries.setRgb(0,0,0);
    cCalculatedEntries.setRgb(255,0,0);
    displayFont.setFamily("Arial");

    wMajorGrid = 2;
    wMinorGrid = 1;
    selx = -1; sely = -1;
    cLanguageOffset = 0x0030;
    dCellSize = 15;
    nFontSize = 24;
    qsrand ( QTime::currentTime().msec() );

    setOriginalCell(0,0,1);
    setOriginalCell(0,1,3);
    setOriginalCell(0,4,9);
    setOriginalCell(0,7,5);
    setOriginalCell(1,1,4);
    setOriginalCell(1,3,1);
    setOriginalCell(1,4,6);
    setOriginalCell(2,0,2);
    setOriginalCell(2,1,6);
    setOriginalCell(2,5,5);
    setOriginalCell(2,8,1);
    setOriginalCell(3,1,1);
    setOriginalCell(3,3,5);
    setOriginalCell(3,4,2);
    setOriginalCell(3,6,6);
    setOriginalCell(4,0,9);
    setOriginalCell(4,2,6);
    setOriginalCell(4,6,7);
    setOriginalCell(4,8,4);
    setOriginalCell(5,2,4);
    setOriginalCell(5,4,7);
    setOriginalCell(5,5,9);
    setOriginalCell(5,7,1);
    setOriginalCell(6,0,4);
    setOriginalCell(6,3,8);
    setOriginalCell(6,7,6);
    setOriginalCell(6,8,5);
    setOriginalCell(7,4,5);
    setOriginalCell(7,5,1);
    setOriginalCell(7,7,4);
    setOriginalCell(8,1,9);
    setOriginalCell(8,4,4);
    setOriginalCell(8,7,8);
    setOriginalCell(8,8,3);
}

SudokuWidget::SudokuWidget(int grd[][9])
{
    clearAll();
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            if(grd[i][j] != -1)
                setOriginalCell(i,j,grd[i][j]);
}

void SudokuWidget::save()
{
    int i, j;
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save File"), "", tr("Sudoku Files (*.sdk)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("Codecs"),tr("Cannot write file %1:\n%2").arg(fileName).arg(file.errorString()));
            return;
        }
        else
        {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            for(i=0; i<9; i++)
            {
                for(j=0; j<9; j++)
                {
                    out << cellValue(i,j) << " ";
                }
            }
        }
    }
}

void SudokuWidget::load()
{
    int i, j, tmp;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), "", tr("Sudoku Files (*.sdk)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("Codecs"),tr("Cannot read file %1:\n%2").arg(fileName).arg(file.errorString()));
            return;
        }
        else
        {
            clearAll();

            QTextStream out(&file);
            out.setCodec("UTF-8");
            for(i=0; i<9; i++)
            {
                for(j=0; j<9; j++)
                {
                    out >> tmp;
                    if(tmp!=-1)
                    {
                        setOriginalCell(i,j,tmp);
                    }
                }
            }
        }
    }
    update();
}

void SudokuWidget::updateFont(const QString &font)
{
    displayFont.setFamily(font);
    adjustSize();
    update();
}

QSize SudokuWidget::sizeHint() const
{
    return QSize(500,500);
}

void SudokuWidget::mouseReleaseEvent( QMouseEvent * event )
{
    int x=-1,y=-1;
    if(event->x() > margin && event->x()<width()-margin)
    {
        for(int i = 8; i >= 0; i--)
        {
            if( event->x() > (margin+i*cellwidth) )
            {
                y = i;
                break;
            }
        }
    }

    if(event->y() > margin && event->y()<width()-margin)
    {
        for(int i = 8; i >= 0; i--)
        {
            if( event->y() > (margin+i*cellwidth) )
            {
                x = i;
                break;
            }
        }
    }

    if(x==selx && y==sely)
    {
        selx=-1; sely=-1;
    }
    else
    {
        selx = x; sely = y;
    }

    update();
    QWidget::mouseReleaseEvent(event);
}

void SudokuWidget::keyReleaseEvent( QKeyEvent * event )
{
    if(selx!=-1 && sely!=-1)
    {
        if(event->text()==" ")
        {
            setOriginalCell(selx,sely,-1);
        }
        else
        {
            int value = event->text().toInt();
            if(value >=1 && value <= 9)
            {
                setOriginalCell(selx,sely,value);
            }
        }
        update();
    }
    QWidget::keyReleaseEvent(event);
}

void SudokuWidget::paintEvent(QPaintEvent *event)
{
    int value;

    // assume width and height are identical
    gridwidth = ( (this->width() - 2*margin) / 9 ) * 9;
    gridheight = ( (this->width() - 2*margin) / 9 ) * 9;
    cellwidth = gridwidth / 9;

    QPainter painter(this);
    painter.fillRect(event->rect(), QBrush(cBackground));

    displayFont.setPixelSize(cellwidth);
    painter.setFont(displayFont);

    // draw selection box
    if(selx!=-1 && sely!=-1)
        painter.fillRect(margin+sely*cellwidth,margin+selx*cellwidth,cellwidth,cellwidth,cCellBackground);

    // draw minor grid lines
    for(int i=0; i<10; i++)
    {
        if(i%3==0)
            painter.setPen(QPen(QBrush(cMajorGrid,Qt::SolidPattern),wMajorGrid));
        else
            painter.setPen(QPen(QBrush(cMinorGrid,Qt::SolidPattern),wMinorGrid));
        painter.drawLine(margin,margin+i*cellwidth,margin+9*cellwidth,margin+i*cellwidth);
    }
    for(int i=0; i<10; i++)
    {
        if(i%3==0)
            painter.setPen(QPen(QBrush(cMajorGrid,Qt::SolidPattern),wMajorGrid));
        else
            painter.setPen(QPen(QBrush(cMinorGrid,Qt::SolidPattern),wMinorGrid));
        painter.drawLine(margin+i*cellwidth,margin,margin+i*cellwidth,margin+9*cellwidth);
    }

    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            value = cellValue(i,j);
            if(value!=-1)
            {
                if(originalValue(i,j))
                    painter.setPen(cGivenEntries);
                else
                    painter.setPen(cCalculatedEntries);

                painter.drawText(margin+j*cellwidth,margin+i*cellwidth,cellwidth,cellwidth,Qt::AlignHCenter|Qt::AlignVCenter,QString(QChar(cLanguageOffset + value)));
            }
        }
    }


    return;
}

void SudokuWidget::solvePuzzle()
{
    if(!solve(false,false))
        QMessageBox::critical(0,"Error","The solver seems not to be making progress. Maybe there is something wrong with the program, or maybe there is something wrong with your input.");
    update();
}

QFont SudokuWidget::font()
{
    return displayFont;
}

void SudokuWidget::oneSolutionStep()
{
    //    stochasticSolutionStep();
    solutionStep(true,false);
    update();
}

void SudokuWidget::changeLanguage(QAction *action)
{
    cLanguageOffset = action->data().toUInt();
    update();
}

void SudokuWidget::saveEmptyPuzzle()
{
    saveSvg(false);
}

void SudokuWidget::saveSolvedPuzzle()
{
    saveSvg(true);
}

void SudokuWidget::saveSvg(bool withAnswers)
{
    double dInitial = dCellSize / 2.0;

    qreal dWidth = 9 * dCellSize;
    qreal dHeight = 9 * dCellSize;

    qreal xpos = dInitial;
    qreal ypos = dInitial + dCellSize / 2;
    if( cLanguageOffset == 0x0030 ) // Western
        ypos = dInitial + dCellSize / 3;

    QString filename = QFileDialog::getSaveFileName(this,tr("Sudoku"),QString(),"SVG Files (*.svg)");
    if(filename.isEmpty())
        return;

    if(withAnswers)
    {
        solve(false,false);
        update();
    }

    QFile file(filename);
    if(!file.open(QFile::WriteOnly|QFile::Text))
        return;
    QXmlStreamWriter w(&file);

    w.setCodec("utf-8");
    w.writeStartDocument("1.0");
    w.setAutoFormatting(true);

    w.writeStartElement("svg");
    w.writeDefaultNamespace("http://www.w3.org/2000/svg");
    w.writeNamespace ( "http://www.w3.org/1999/xlink", "xlink" );
    w.writeAttribute("width",QString("%1mm").arg(dWidth));
    w.writeAttribute("height",QString("%1mm").arg(dHeight));
    w.writeAttribute("version","1.1");

    w.writeStartElement("g");

    w.writeEmptyElement("path");
    w.writeAttribute("transform","scale(3.543307)");
    QString path = "";
    for(int i=0; i<10; i++)
    {
        // horizontal
        path += QString("M %1,%2 %3,%4 ").arg(0).arg( i * dCellSize ).arg( 9*dCellSize ).arg( i * dCellSize );
        // vertical
        path += QString("M %1,%2 %3,%4 ").arg(i * dCellSize).arg( 0 ).arg( i * dCellSize  ).arg( 9*dCellSize );
    }
    w.writeAttribute("style","stroke-linecap:round;stroke:#000000;stroke-linecap:butt;stroke-opacity:1.0000000;stroke-width:0.5px;");
    w.writeAttribute("d",path);

    w.writeEmptyElement("path");
    w.writeAttribute("transform","scale(3.543307)");
    path = "";
    for(int i=0; i<10; i+=3)
    {
        // horizontal
        path += QString("M %1,%2 %3,%4 ").arg(0).arg( i * dCellSize ).arg( 9*dCellSize ).arg( i * dCellSize );
        // vertical
        path += QString("M %1,%2 %3,%4 ").arg(i * dCellSize).arg( 0 ).arg( i * dCellSize  ).arg( 9*dCellSize );
    }
    w.writeAttribute("style","stroke-linecap:round;stroke:#000000;stroke-linecap:butt;stroke-opacity:1.0000000;stroke-width:1px;");
    w.writeAttribute("d",path);

    w.writeEndElement(); // g

    w.writeStartElement("g");

    for(int i=0; i < 9; i++)
    {
        for(int j=0; j < 9; j++)
        {
            if( withAnswers || originalValue(i,j) )
            {
                w.writeStartElement("text");
                w.writeAttribute("style",QString("text-anchor:middle; font-family: %1; font-size: %2mm;").arg(displayFont.family()).arg( dCellSize ));
                w.writeAttribute("x",QString("%1mm").arg(xpos));
                w.writeAttribute("y",QString("%1mm").arg(ypos));

                if(withAnswers)
                    w.writeTextElement("tspace", QString(QChar(cLanguageOffset + cellValue(i,j))) );
                else if( !withAnswers && originalValue(i,j) )
                    w.writeTextElement("tspace", QString(QChar(cLanguageOffset + cellValue(i,j))) );

                w.writeEndElement(); // text;
            }
            xpos += dCellSize;
        }
        xpos = dInitial;
        ypos += dCellSize;
    }
    w.writeEndElement(); // g

    w.writeEndElement(); // svg

    w.writeEndDocument();
}

void SudokuWidget::populateGridRandomly()
{
    clearAll();
    solve(true,true);
    /*
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            int r = randomUpTo( numberOfPossibleValues(i,j) );
//            setOriginalCell(i,j,findValue(i,j,r));
            setOriginalCell(i,j,findValue(i,j));
        }
    }
*/
    setNonEmptyToOriginal();
    update();
}

void SudokuWidget::createPuzzle()
{
    bool ok;
    double proportion = QInputDialog::getDouble(this, tr("Create a random puzzle"),
                                       tr("Proportion of cells filled:"), 0.5, 0, 1, 2, &ok);
    if (ok)
    {
        clearAll();
        solve(true,true);

        for(int i=0; i<9; i++)
        {
            for(int j=0; j<9; j++)
            {
                if( proportion < random() )
                {
                    grid[i][j] = -1;
                    original[i][j] = false;
                }
            }
        }

        setNonEmptyToOriginal();
        update();
    }
}

void SudokuWidget::randomPopulationStep()
{
    int ct = 0;
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            int x = 9*i + j%3;
            int y = 27 * (i%9) + j/3;
            if( grid[x][y] == -1 )
            {
                setOriginalCell(x,y,ct);
                ct++;
                /*
                int r = randomUpTo( numberOfPossibleValues(x,y) );
                qDebug() << QString("(%1,%2)").arg(i).arg(j) << r << numberOfPossibleValues(x,y);
                r = 1;
                setOriginalCell(x,y,findValue(x,y,r));
                update();
                return;
*/
            }
        }
    }
}

void SudokuWidget::setOriginalCell(int x, int y, int value)
{
    grid[x][y] = value;
    original[x][y] = true;
}

void SudokuWidget::setCalculatedCell(int x, int y, int value)
{
    grid[x][y] = value;
    original[x][y] = false;
}

int SudokuWidget::numberOfPossibleValues(int x, int y) const
{
    int count=0;
    for(int i=1; i<=9; i++)
        if(!rowContains(x,i) && !colContains(y,i) && !boxContains(x,y,i))
            count++;
    return count;
}

int SudokuWidget::findValue(int x, int y, int number) const
{
    int ct=0;
    for(int i=1; i<=9; i++) // i is cycling through candidate cell values
    {
        //        qDebug() << "Suppose" << i;
        if(!rowContains(x,i) && !colContains(y,i) && !boxContains(x,y,i))
        {
            ct++;
            //          qDebug() << "yes" << ct << number;
            if(ct==number)
                return i;
        }
        /*
        else
        {
            qDebug() << "no";
        }
*/
    }
    return -1;
}

bool SudokuWidget::rowContains(int x, int value) const
{
    for(int i=0; i<9; i++)
        if(grid[x][i]==value)
            return true;
    return false;
}

bool SudokuWidget::colContains(int y, int value) const
{
    for(int i=0; i<9; i++)
        if(grid[i][y]==value)
            return true;
    return false;
}

bool SudokuWidget::boxContains(int x, int y, int value) const
{
    int fromx, tox, fromy, toy;
    if(x<6)
    {
        if(x<3)
        {
            fromx = 0;
            tox = 2;
        }
        else
        {
            fromx = 3;
            tox = 5;
        }
    }
    else
    {
        fromx = 6;
        tox = 8;
    }

    if(y<6)
    {
        if(y<3)
        {
            fromy = 0;
            toy = 2;
        }
        else
        {
            fromy = 3;
            toy = 5;
        }
    }
    else
    {
        fromy = 6;
        toy = 8;
    }

    return rangeContains(fromx,tox,fromy,toy,value);
}

bool SudokuWidget::rangeContains(int fromx, int tox, int fromy, int toy, int value) const
{
    for(int i=fromx; i<=tox; i++)
        for(int j=fromy; j<=toy; j++)
            if(grid[i][j]==value)
                return true;
    return false;
}

bool SudokuWidget::solutionStep(bool justOne, bool stochastic)
{
    int candidate, nposs, minpossibilities=9, mini=0, minj=0;
    int nSolutions = 0;
    int nTargetSolution = randomUpTo( numberOfPossibleMoves() ); // only used if it's stochastic

    aPossibleMoveHistory << 100.0 * ( (double)numberOfPossibleMoves() / (double)numberOfUnknownCells() );
    aSteps << aPossibleMoveHistory.count();

    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            if(cellValue(i,j)==-1) // if it's an empty cell
            {
                nposs = numberOfPossibleValues(i,j);
//                qDebug() << i << j << nposs;

                if(nposs < minpossibilities)
                {
                    minpossibilities = nposs;
                    mini = i;
                    minj = j;
                }
                if(nposs==1)
                {
                    nSolutions++;
                    //                    qDebug() << nSolutions << which;
                    if( !stochastic || nSolutions == nTargetSolution )
                    {
                        //                        qDebug() << "setting cell" << i << j << "to" << findValue(i,j);
                        setCalculatedCell(i,j,findValue(i,j));
                        if(justOne)
                            return true;
                    }
                }
                else if(nposs==0)
                    return false;
            }
        }
    }
    if(minpossibilities>1) // must be hard
    {
//        qDebug() << mini << minj << minpossibilities;
        for(int i=1; i<=minpossibilities; i++) // cycle through all possibilities
        {
            candidate = findValue(mini,minj,i);
            SudokuWidget *hyp = new SudokuWidget(grid);
            hyp->setOriginalCell(mini,minj,candidate);

            if(hyp->solve(false,false)) // this solution is non-stochastic; we will select which cell to copy randomly below
            {
//                qDebug() << "hypothetical success" << hyp << nBifurcationDepth;

                if( justOne )
                {
                    int x, y;
                    if(stochastic)
                        nthUnknownCell(x,y,randomUpTo(numberOfUnknownCells()) );
                    else
                        nthUnknownCell(x,y);

                    setCalculatedCell(x,y,hyp->cellValue(x,y));
                }
                else
                {
                    copySolutionFromGrid(hyp->grid);
                }
                return true;
            }
            delete hyp;
        }
    }
    return true;
}

void SudokuWidget::copySolutionFromGrid(int grd[][9])
{
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            if(!originalValue(i,j))
                setCalculatedCell(i,j,grd[i][j]);
}

bool SudokuWidget::solve(bool stepwise, bool stochastic)
{
    clearCalculated();

    int last=81, current;
    do
    {
        if(stochastic)
        {
            if(!solutionStep(true,true))
                return false;
        }
        else
        {
            if(!solutionStep(stepwise,false))
                return false;
        }
        current = numberOfUnknownCells();
        if(current==last)
            return false;
        last = current;
    }
    while(current>0);

    if(numberOfUnknownCells()==0)
        return true;
    else
        return false;
}

int SudokuWidget::cellValue(int x, int y) const
{
    return grid[x][y];
}

bool SudokuWidget::originalValue(int x, int y) const
{
    return original[x][y];
}

int SudokuWidget::numberOfUnknownCells() const
{
    int count = 0;
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            if(grid[i][j]==-1)
                count++;
    return count;
}

void SudokuWidget::clearCalculated()
{
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            if(!original[i][j])
                grid[i][j] = -1;
    update();
}

void SudokuWidget::clearAll()
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            grid[i][j] = -1;
            original[i][j] = false;
        }
    }
    update();
}

int SudokuWidget::randomUpTo(int ceiling) const
{
    return 1 + qRound( ((qreal)qrand()/(qreal)RAND_MAX) * (qreal)(ceiling-1));
}

double SudokuWidget::random() const
{
    return (double)qrand()/(double)RAND_MAX;
}

void SudokuWidget::nthUnknownCell(int &i, int &j, int which) const
{
    int count = 0;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            if( grid[i][j] == -1 )
            {
                count++;
                if(count == which)
                    return;
            }
        }
    }
}

void SudokuWidget::setNonEmptyToOriginal()
{
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            if( grid[i][j] != -1 )
                original[i][j] = true;
}

int SudokuWidget::numberOfPossibleMoves() const
{
    int nPossibleMoves = 0;
    for(int i=0; i<9; i++)
        for(int j=0; j<9; j++)
            if(cellValue(i,j)==-1) // if it's an empty cell
                if( numberOfPossibleValues(i,j) == 1 )
                    nPossibleMoves++;
    return nPossibleMoves;
}

void SudokuWidget::solvePuzzleWithReport()
{
    aSteps.clear();
    aPossibleMoveHistory.clear();

    solve(true,false);

    QwtPlot *qwtPlot = new QwtPlot;
    qwtPlot->enableAxis(QwtPlot::yLeft);

    QwtPlotCurve *waveCurve = new QwtPlotCurve("dummy");
    waveCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    waveCurve->setPen(QPen(Qt::blue));
    waveCurve->setData(aSteps,aPossibleMoveHistory);
    waveCurve->attach(qwtPlot);

    qwtPlot->setWindowTitle("Available solutions over time");
    qwtPlot->setAxisTitle(QwtPlot::yLeft,"Pecentage of Empty Cells with Deducible Numbers");
    qwtPlot->setAxisTitle(QwtPlot::xBottom,"Solution Steps");

    QVector<double> aBifurcationPoints;
    QVector<double> aZero;

    int index = -1;
    while( (index = aPossibleMoveHistory.indexOf( 0, index +1 ) ) != -1  )
    {
        aBifurcationPoints << index + 1;
        aZero << 0;
    }

    if(aBifurcationPoints.count() > 0)
    {
        QwtPlotCurve *pointsCurve = new QwtPlotCurve("bifurcation points");
        pointsCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
        pointsCurve->setStyle(QwtPlotCurve::Dots);
        pointsCurve->setSymbol(QwtSymbol(QwtSymbol::XCross,QBrush(Qt::red),QPen(Qt::red),QSize(7,7)));
        pointsCurve->setData(aBifurcationPoints,aZero);
        pointsCurve->attach(qwtPlot);
    }

    qwtPlot->setWindowFlags(Qt::Window);
    qwtPlot->resize(640,480);
    qwtPlot->show();

    aSteps.clear();
    aPossibleMoveHistory.clear();
}
