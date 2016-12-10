#ifndef BINARYOUTPUTREPRESENTATION_H
#define BINARYOUTPUTREPRESENTATION_H

#include <vector>
#include "../core/graphicobject.h"
#include "../ann_gui/mlpobject.h"

using namespace std;

class MLPObject;

/*!
 * \brief The BinaryOutputElement class
 *
 * \deprecated Replaced by DotMatrix
 *
 * \author Edixon Vargas <ingedixonvargas@gmail.com>
 * \date 02/02/2015
 */
class BinaryOutputElement : public GraphicObject
{
	public:

		enum{BinaryOutputRepresentationType = UserType + 4};

		enum Align{
			Horizontal,
			Vertical
		};

		enum ThresholdType{
			LESS_THAN,
			LESS_EQUAL_THAN,
			GREATER_THAN,
			GREATER_EQUAL_THAN,
			GREATER_THAN_ALL,
			LESS_THAN_ALL
		};

		explicit BinaryOutputElement(int inputs, Align a = Vertical, double threshold = 0, ThresholdType tt = GREATER_THAN);

		void setInputs(const QVector<double> &inputs);
		QVector<double> getInputs();
		QVector<bool> getBinaryInputs();

		void setAligment(Align a);
		Align getAligment();

		void setThresholdType(ThresholdType tt);
		void setThreshold(double threshold);

		void setInputElement(GraphicObject *ge);

		/**
		  Vease funcion @code{type()} de la clase @code{QGraphicsItem}

		  @see type
		  */
		int type() const;

	protected:

		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	protected slots:

		void onInputChanged(QVector<double> input);
		void propertyClick();
		void openOnWindowClick();

	private:
		Q_OBJECT

		double threshold;
		ThresholdType tt;
		QVector<double> inputs;
		Align aligment;
		QRectF intRect;

		void initBOR(int inputs, Align a = Vertical, double threshold = 0, ThresholdType tt = GREATER_THAN);
		void setInputSize(int inputs);
};

#endif // BINARYOUTPUTREPRESENTATION_H
