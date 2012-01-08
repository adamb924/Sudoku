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

void SudokuWidget::updateFont(const QFont &font)
{
    displayFont.setFamily(font.family());
    adjustSize();
    update();
}

QSize SudokuWidget::sizeHint() const
{
    return QSize(500,500);
}

void SudokuWidget::mouseReleaseEvent( QMouseEvent * event )
{
    int i, x,y;
    if(event->x() > margin && event->x()<width()-margin)
    {
	for(i = 8; i >= 0; i--)
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
	for(i = 8; i >= 0; i--)
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
    {
	painter.fillRect(margin+sely*cellwidth,margin+selx*cellwidth,cellwidth,cellwidth,cCellBackground);
    }

    // draw minor grid lines
    for(i=0; i<10; i++)
    {
	if(i%3==0)
	{
	    painter.setPen(QPen(QBrush(cMajorGrid,Qt::SolidPattern),wMajorGrid));
	}
	else
	{
	    painter.setPen(QPen(QBrush(cMinorGrid,Qt::SolidPattern),wMinorGrid));
	}
	painter.drawLine(margin,margin+i*cellwidth,margin+9*cellwidth,margin+i*cellwidth);
    }
    for(i=0; i<10; i++)
    {
	if(i%3==0)
	{
	    painter.setPen(QPen(QBrush(cMajorGrid,Qt::SolidPattern),wMajorGrid));
	}
	else
	{
	    painter.setPen(QPen(QBrush(cMinorGrid,Qt::SolidPattern),wMinorGrid));
	}
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
		{
		    painter.setPen(cGivenEntries);
		}
		else
		{
		    painter.setPen(cCalculatedEntries);
		}
		painter.drawText(margin+j*cellwidth,margin+i*cellwidth,cellwidth,cellwidth,Qt::AlignHCenter|Qt::AlignVCenter,QString::number(value));
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
    {
	QMessageBox::critical(0,"Error","The solver seems not to be making progress. Maybe there is something wrong with the program, or maybe there is something wrong with your input.");
    }
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


void SudokuGrid::setOriginalCell(int x, int y, int value)
{
    grid[x][y] = value;
    original[x][y] = true;
}

void SudokuGrid::setCalculatedCell(int x, int y, int value)
{
    if(!original[x][y])
    {
	grid[x][y] = value;
	original[x][y] = false;
    }
}

int SudokuGrid::numberOfPossibleValues(int x, int y)
{
    int i, count=0;
    for(i=1; i<=9; i++)
    {
	if(!rowContains(x,i) && !colContains(y,i) && !boxContains(x,y,i))
	{
	    count++;
	}
    }
    return count;
}

int SudokuGrid::findValue(int x, int y, int number=1)
{
    int i, ct=0;
    for(i=1; i<=9; i++)
    {
	if(!rowContains(x,i) && !colContains(y,i) && !boxContains(x,y,i))
	{
	    ct++;
	    if(ct==number)
	    {
		return i;
	    }
	}
    }
    return -1;
}

bool SudokuGrid::rowContains(int x, int value)
{
    int j;
    for(j=0; j<9; j++)
    {
	if(grid[x][j]==value)
	{
	    return true;
	}
    }
    return false;
}

bool SudokuGrid::colContains(int y, int value)
{
    int i;
    for(i=0; i<9; i++)
    {
	if(grid[i][y]==value)
	{
	    return true;
	}
    }
    return false;
}

bool SudokuGrid::boxContains(int x, int y, int value)
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

bool SudokuGrid::rangeContains(int fromx, int tox, int fromy, int toy, int value)
{
    int i, j;
    for(i=fromx; i<=tox; i++)
    {
//	for(j=fromy; j<toy; j++)
	for(j=fromy; j<=toy; j++)
	{
	    if(grid[i][j]==value)
	    {
		return true;
	    }
	}
    }
    return false;
}

bool SudokuGrid::solutionStep(bool justOne=false)
{
    int i, j, k, candidate, nposs, minpossibilities=81, mini, minj;
    for(i=0; i<9; i++)
    {
	for(j=0; j<9; j++)
	{
	    if(cellValue(i,j)==-1)
	    {
		nposs = numberOfPossibleValues(i,j);

		if(nposs < minpossibilities)
		{
		    minpossibilities = nposs;
		    mini = i;
		    minj = j;
		}
		if(nposs==1)
		{
		    setCalculatedCell(i,j,findValue(i,j));
		    if(justOne) { return true; }
		}
		if(nposs==0)
		{
		    return false;
		}
	    }
	}
    }
    if(minpossibilities>1) // must be hard
    {
	for(k=1; k<=minpossibilities; k++) // cycle through all possibilities
	{
	    candidate = findValue(mini,minj,k);
	    SudokuGrid *hyp = new SudokuGrid(grid);
	    hyp->setOriginalCell(mini,minj,candidate);

	    if(hyp->solve())
	    {
		copySolutionFromGrid(hyp->grid);
//		setCalculatedCell( mini,minj,hyp->cellValue(mini,minj) );
//		delete hyp;
		return true;
	    }
	    delete hyp;
	}
    }
    return true;
}

void SudokuGrid::copySolutionFromGrid(int grd[][9])
{
    int i, j;
    for(i=0; i<9; i++)
    {
	for(j=0; j<9; j++)
	{
	    if(!originalValue(i,j))
	    {
		setCalculatedCell(i,j,grd[i][j]);
	    }
	}
    }
}

bool SudokuGrid::solve()
{
    int last=81, current;
    do
    {
	if(!solutionStep())
	{
	    return false;
	}
	current = numberOfUnknownCells();
	if(current==last)
	{
	    return false;
	}
	last = current;
    }
    while(current>0);
    if(numberOfUnknownCells()==0)
    {
	return true;
    }
    else
    {
	return false;
    }
}

int SudokuGrid::cellValue(int x, int y)
{
    return grid[x][y];
}

bool SudokuGrid::originalValue(int x, int y)
{
    return original[x][y];
}

int SudokuGrid::numberOfUnknownCells()
{
    int i, j, count = 0;
    for(i=0; i<9; i++)
    {
	for(j=0; j<9; j++)
	{
	    if(grid[i][j]==-1)
	    {
		count++;
	    }
	}
    }
    return count;
}

void SudokuGrid::clearCalculated()
{
    int i, j;
    for(i=0; i<9; i++)
    {
	for(j=0; j<9; j++)
	{
	    if(!original[i][j])
	    {
		grid[i][j] = -1;
	    }
	}
    }

}

void SudokuGrid::clearAll()
{
    int i, j;
    for(i=0; i<9; i++)
    {
	for(j=0; j<9; j++)
	{
	    grid[i][j] = -1;
	    original[i][j] = false;
	}
    }
}
