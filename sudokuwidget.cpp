#include <QtGui>

#include "sudokuwidget.h"

SudokuWidget::SudokuWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);

    margin = 10;

    cBackground.setRgb(255,255,255);
    cCellBackground.setRgb(225,225,225);
    cMinorGrid.setRgb(62,157,255);
    cMajorGrid.setRgb(0,0,255);
    cGivenEntries.setRgb(0,0,0);
    cCalculatedEntries.setRgb(255,0,0);
    displayFont.setFamily("Arial");

    sg.setOriginalCell(0,0,1);
    sg.setOriginalCell(0,1,3);
    sg.setOriginalCell(0,4,9);
    sg.setOriginalCell(0,7,5);
    sg.setOriginalCell(1,1,4);
    sg.setOriginalCell(1,3,1);
    sg.setOriginalCell(1,4,6);
    sg.setOriginalCell(2,0,2);
    sg.setOriginalCell(2,1,6);
    sg.setOriginalCell(2,5,5);
    sg.setOriginalCell(2,8,1);
    sg.setOriginalCell(3,1,1);
    sg.setOriginalCell(3,3,5);
    sg.setOriginalCell(3,4,2);
    sg.setOriginalCell(3,6,6);
    sg.setOriginalCell(4,0,9);
    sg.setOriginalCell(4,2,6);
    sg.setOriginalCell(4,6,7);
    sg.setOriginalCell(4,8,4);
    sg.setOriginalCell(5,2,4);
    sg.setOriginalCell(5,4,7);
    sg.setOriginalCell(5,5,9);
    sg.setOriginalCell(5,7,1);
    sg.setOriginalCell(6,0,4);
    sg.setOriginalCell(6,3,8);
    sg.setOriginalCell(6,7,6);
    sg.setOriginalCell(6,8,5);
    sg.setOriginalCell(7,4,5);
    sg.setOriginalCell(7,5,1);
    sg.setOriginalCell(7,7,4);
    sg.setOriginalCell(8,1,9);
    sg.setOriginalCell(8,4,4);
    sg.setOriginalCell(8,7,8);
    sg.setOriginalCell(8,8,3);

    wMajorGrid = 2;
    wMinorGrid = 1;

    selx = -1; sely = -1;

    cLanguageOffset = 0x0030;

    dCellSize = 15;
    nFontSize = 24;
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
		    out << sg.cellValue(i,j) << " ";
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
			sg.setOriginalCell(i,j,tmp);
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
	    sg.setOriginalCell(selx,sely,-1);
	}
	else
	{
	    int value = event->text().toInt();
	    if(value >=1 && value <= 9)
	    {
		sg.setOriginalCell(selx,sely,value);
	    }
	}
	update();
    }
    QWidget::keyReleaseEvent(event);
}

void SudokuWidget::paintEvent(QPaintEvent *event)
{
    int i, j, value;

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
    for(i=0; i<10; i++)
    {
	if(i%3==0)
	    painter.setPen(QPen(QBrush(cMajorGrid,Qt::SolidPattern),wMajorGrid));
	else
	    painter.setPen(QPen(QBrush(cMinorGrid,Qt::SolidPattern),wMinorGrid));
	painter.drawLine(margin,margin+i*cellwidth,margin+9*cellwidth,margin+i*cellwidth);
    }
    for(i=0; i<10; i++)
    {
	if(i%3==0)
	    painter.setPen(QPen(QBrush(cMajorGrid,Qt::SolidPattern),wMajorGrid));
	else
	    painter.setPen(QPen(QBrush(cMinorGrid,Qt::SolidPattern),wMinorGrid));
	painter.drawLine(margin+i*cellwidth,margin,margin+i*cellwidth,margin+9*cellwidth);
    }

    for(i=0; i<9; i++)
    {
	for(j=0; j<9; j++)
	{
	    value = sg.cellValue(i,j);
	    if(value!=-1)
	    {
		if(sg.originalValue(i,j))
		    painter.setPen(cGivenEntries);
		else
		    painter.setPen(cCalculatedEntries);

                //                qDebug() << value << cLanguageOffset << cLanguageOffset + value;
                painter.drawText(margin+j*cellwidth,margin+i*cellwidth,cellwidth,cellwidth,Qt::AlignHCenter|Qt::AlignVCenter,QString(QChar(cLanguageOffset + value)));
	    }
	}
    }


    return;
}

void SudokuWidget::solutionStep(bool justOne=false)
{
    sg.solutionStep(justOne);
    update();
}

void SudokuWidget::solve()
{
    if(!sg.solve())
	QMessageBox::critical(0,"Error","The solver seems not to be making progress. Maybe there is something wrong with the program, or maybe there is something wrong with your input.");
    update();
}

QFont SudokuWidget::font()
{
    return displayFont;
}

void SudokuWidget::clearCalculated()
{
    sg.clearCalculated();
    update();
}

void SudokuWidget::clearAll()
{
    sg.clearAll();
    update();
}

void SudokuWidget::oneSolutionStep()
{
    solutionStep(true);
}

void SudokuWidget::changeLanguage(QAction *action)
{
    switch(action->data().toInt())
    {
    case Western:
        cLanguageOffset = 0x0030;
        break;
    case Arabic:
        cLanguageOffset = 0x0660;
        break;
    case Persian:
        cLanguageOffset = 0x06F0;
        break;
    case Devanagari:
        cLanguageOffset = 0x0966;
        break;
    case Bengali:
        cLanguageOffset = 0x09E6;
        break;
    default: // Western
        cLanguageOffset = 0x0030;
        break;
    }

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
        sg.solve();
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

    for(int i=0; i < 9; i++)
    {
        for(int j=0; j < 9; j++)
        {
            if( withAnswers || sg.originalValue(i,j) )
            {
                w.writeStartElement("text");
                w.writeAttribute("style",QString("text-anchor:middle; font-family: %1; font-size: %2mm;").arg(displayFont.family()).arg( dCellSize ));
                w.writeAttribute("x",QString("%1mm").arg(xpos));
                w.writeAttribute("y",QString("%1mm").arg(ypos));

                if(withAnswers)
                    w.writeTextElement("tspace", QString(QChar(cLanguageOffset + sg.cellValue(i,j))) );
                else if( !withAnswers && sg.originalValue(i,j) )
                    w.writeTextElement("tspace", QString(QChar(cLanguageOffset + sg.cellValue(i,j))) );

                w.writeEndElement(); // text;
            }
            xpos += dCellSize;
        }
        xpos = dInitial;
        ypos += dCellSize;
    }

    w.writeEndElement(); // svg

    w.writeEndDocument();
}
