/*
CT Matrix
Copyright (c) 2016 Christopher Thompson
*/
#ifndef CTM_H_
#define CTM_H_

#include <climits>
#include <cmath>
#include <iostream>

namespace ct {
	typedef unsigned uns;
	typedef unsigned long luns;
	enum DStatus {ROW, COL};
}

/*
VIEW CLASS DECLARATION
*/
namespace ct {
	template <class T>
	class View {
	public:
		typedef T* pvc;
	private:
		// Members
		pvc mBegin;
		uns mRows;
		uns mCols;
		DStatus mStatus;
	public:
		// Constructors
		View();
		View(const View & v);
		View(const pvc & begin, const pvc & end, const uns & rows, const uns & cols, const DStatus & stat=COL);
		// Destructor
		virtual ~View() {}
		// Assignment
		View & operator=(const View & v);
		// Property Access
		const pvc begin() const {return mBegin;}
		const pvc end() const;
		const uns & rows() const {return mRows;}
		const uns & cols() const {return mCols;}
		const DStatus & status() const {return mStatus;}
		// Comparison
		bool operator==(const View & v) const;
	};
} // END VIEW DECLARATION

/*
VIEW CLASS DEFINITION
*/
namespace ct {

// Constructors

template <class T>
View<T>::View() : mBegin(0) {
	mRows = 0;
	mCols = 0;
	mStatus = COL;
}

template <class T>
View<T>::View(const View<T> & v) : mBegin(v.mBegin){
	mRows = v.mRows;
	mCols = v.mCols;
	mStatus = v.mStatus;
}

template <class T>
View<T>::View(const pvc & begin, const pvc & end, const uns & rows,const uns & cols, const DStatus & stat) {
	mBegin = begin;
	
	uns count = 0;
	bool ok = true;
	pvc it = mBegin;
	for(;it != end; it++) {
		count++;
	}
	
	if(stat==COL && count!=cols) {
		std::cout << "[-] Error: length between 'begin' and 'end' must be 'cols'. Should be " << count << std::endl;
		ok = false;
	}
	else if(stat==ROW && count!=rows) {
		std::cout << "[-] Error: length between 'begin' and 'end' must be 'rows'. Should be " << count << std::endl;
		ok = false;
	}
	
	if(!ok) {
		std::cout << "Initializing mBegin for object at " << this << " to 'NULL'." << std::endl;
		mBegin = 0;
		mRows = 0;
		mCols = 0;
		mStatus = COL;
		return;
	}
	
	mRows = rows;
	mCols = cols;
	mStatus = stat;
}

// Assignment

template <class T>
View<T> & View<T>::operator=(const View<T> & v) {
	if(this == v)
		return *this;
	mBegin = v.mBegin;
	mRows = v.mRows;
	mCols = v.mCols;
	mStatus = v.mStatus;
	
	return *this;
}

// Property Access

template <class T>
const typename View<T>::pvc View<T>::end() const {
	if(mStatus == COL)
		return mBegin + mCols;
	return mBegin + mRows;
}

// Comparison

template <class T>
bool View<T>::operator==(const View & v) const {
	if(mBegin==v.mBegin && mRows==v.mRows && mCols==v.mCols && mStatus==v.mStatus)
		return true;
	return false;
}

} // END VIEW DEFINITION

/*
CTM TEMPLATE CLASS DECLARATION
*/
namespace ct {	
	template <class T>
	class CTM {
	public:
		typedef unsigned long ellen; // element length type
		typedef T* element;
		typedef element* pvec;
		typedef T* iterator;
		typedef const T* const_iterator; // === const iterator
	private:
		// Members
		element mEl;
		pvec mVectors;
		uns mRows;
		uns mCols;
		// Private Member Functions
		uns point(const uns & i, const uns & j) const;
		element copy(const element el) const;
		pvec copy(const pvec vecs) const; // Copies addresses of elements
		pvec makeVectors() const;
	public:
		// Constructors
		CTM();
		CTM(const CTM & c);
		CTM(const_iterator & begin, const_iterator & end, const uns & rows, const uns & cols);
// 		CTM(const View<element> & v);
		// Destructor
		virtual ~CTM();
		// Assignment
		CTM<T> & operator=(const CTM<T> & c);
		// Iterators
		iterator begin() {return mEl;}
		iterator end() {return mEl + mRows * mCols;}
		// Const Iterators
		const_iterator cbegin() const {return mEl;}
		const_iterator cend() const {return mEl + mRows * mCols;}
		// Attributes
		const uns & nrows() const {return mRows;}
		const uns & ncols() const {return mCols;}
		// Scalar
		bool isScalar() const;
		const T & scalar() const;
		// Element Access
		T & at(uns i, uns j) {return mEl[point(i,j)];}
		const T & at(uns i, uns j) const {return mEl[point(i, j)];}
		// Transpose
		View<element> t() const; // construct with mVectors
		// Subset; Includes start and end point
		CTM<T> subset(const uns & start_row, const uns & end_row, const uns & start_col, const uns & end_col) const;
		View<element> subset(const uns & start_col, const uns & end_col) const; // BEWARE
		// Matrix Operations
		CTM<T> operator+(const CTM<T> & c) const;
		CTM<T> operator-(const CTM<T> & c) const;
		// Virtual Matrix Operations
// 		virtual CTM<T> operator*(const CTM<T> & c) const;
		// Friend Matrix Operations Element-wise
		friend CTM<T> operator+(const T & n, const CTM<T> & c0);
		friend CTM<T> operator-(const T & n, const CTM<T> & c0);
		friend CTM<T> operator*(const T & n, const CTM<T> & c0);
		friend CTM<T> operator/(const T & n, const CTM<T> & c0);
		// Comparison
		bool operator==(const CTM<T> & c);
		// Printing
		void show() const;
	};
}

/*
CTM TEMPLATE CLASS DEFINITION
*/
namespace ct {

// Private Member Functions

template <class T>
inline uns CTM<T>::point(const uns & i, const uns & j) const {
	if(i >= mRows || j >= mCols) {
		std::cout << "[-] Error: Out of bounds access. Returning last mEl object" << std::endl;
		return mRows * mCols - 1;
	}
	return j * mRows + i;
}

template <class T>
inline typename CTM<T>::element CTM<T>::copy(const element el) const {
	element o = new T[mRows * mCols];
	for(typename CTM<T>::ellen i = 0; i < mRows * mCols; i++) {
		o[i] = el[i];
	}
	return o;
}

template <class T>
typename CTM<T>::pvec CTM<T>::copy(const pvec vecs) const {
	pvec o = new element[mCols];
	for(uns i = 0; i < mCols; i++) {
		o[i] = vecs[i];
	}
	return o;
}

template <class T>
inline typename CTM<T>::pvec CTM<T>::makeVectors() const {
	uns i = 0;
	pvec o = new element[mCols];
	for(; i < mCols; i++) {
		o[i] = mEl + i * mRows; // &mEl[i * mRows]
	}
	return o;	
}

// Constructors

template <class T>
CTM<T>::CTM() {
	mEl = 0;
	mVectors = 0;
	mRows = 0;
	mCols = 0;
}

template <class T>
CTM<T>::CTM(const CTM & c) {
	mEl = copy(c.mEl);
	mRows = c.mRows;
	mCols = c.mCols;
	
	mVectors = makeVectors();
}

template <class T>
CTM<T>::CTM(const_iterator & begin, const_iterator & end, const uns & rows, const uns & cols) {
	uns i = 0;
	mRows = rows;
	mCols = cols;
	
	mEl = new T[mRows * mCols];
	for(const_iterator it = begin; it != end; it++, i++) {
		mEl[i] = *it;
	}
	
	mVectors = makeVectors();
}

// Destructor

template <class T>
CTM<T>::~CTM() {
	delete [] mVectors;
	delete [] mEl;
}

// Assignment

template <class T>
CTM<T> & CTM<T>::operator=(const CTM<T> & c) {
	if(this == &c)
		return *this;
	
	delete [] mVectors;
	delete [] mEl;
	
	mEl = copy(c.mEl);
	mRows = c.mRows;
	mCols = c.mCols;
	mVectors = makeVectors();
	
	return *this;
}

// Scalar

template <class T>
bool CTM<T>::isScalar() const {
	if(mRows == 1 && mCols == 1)
		return true;
	return false;
}

template <class T>
const T & CTM<T>::scalar() const {
	if(isScalar())
		return mEl[0];
}

// Transpose

template <class T>
View<typename CTM<T>::element> CTM<T>::t() const {
	return View<element>::View(mVectors, mVectors + mCols, mCols, mRows, ROW);
}

// Subset; Includes start and end point

template <class T>
CTM<T> CTM<T>::subset(const uns & start_row, const uns & end_row, const uns & start_col, const uns & end_col) const {
	uns new_rows = end_row - start_row + 1;
	uns new_cols = end_col - start_col + 1;
	
	element o = new T[new_rows * new_cols];
	
	for(uns j = start_col; j <= end_col; j++) {
		for(uns i = start_row; i <= end_row; i++) {
			o[j * new_rows + 1] = point(i, j);
		}
	}
	return CTM<T>::CTM(o, o + new_rows * new_cols, new_rows, new_cols);
}

template <class T>
View<typename CTM<T>::element> CTM<T>::subset(const uns & start_col, const uns & end_col) const {
	return View<T>::View(mVectors + start_col, mVectors + end_col + 1, mRows, end_col - start_col + 1);
}

// Matrix Operations

template <class T>
CTM<T> CTM<T>::operator+(const CTM<T> & c) const {
	if(mRows == c.mRows && mCols == c.mCols) {
		element add_ctm = new T[mRows * mCols];
		uns i = 0;
		const_iterator it = cbegin();
		const_iterator c_it = c.cbegin();
		for(; it != cend(); it++, c_it++, i++) {
			add_ctm[i] = *it + *c_it;
		}
		return CTM<T>::CTM(add_ctm, add_ctm + mRows * mCols, mRows, mCols);
	}
	std::cout << "[-] Error: Addition, 2 matrices must have same dimensions. Returning base constructed matrix." << std::endl;
	return CTM<T>::CTM();
}

template <class T>
CTM<T> CTM<T>::operator-(const CTM<T> & c) const {
	if(mRows == c.mRows && mCols == c.mCols) {
		element sub_ctm = new T[mRows * mCols];
		uns i = 0;
		const_iterator it = cbegin();
		const_iterator c_it = c.cbegin();
		for(; it != cend(); it++, c_it++, i++) {
			sub_ctm[i] = *it + *c_it;
		}
		return CTM<T>::CTM(sub_ctm, sub_ctm + mRows * mCols, mRows, mCols);
	}
	std::cout << "[-] Error: Subtraction, 2 matrices must have same dimensions. Returning base constructed matrix." << std::endl;
	return CTM<T>::CTM();
}

// Friend Matrix Operations Element-wise

template <class T>
CTM<T> operator+(const T & n, const CTM<T> & c) {
	typename CTM<T>::element o = new T[c.mRows * c.mCols];
	for(typename CTM<T>::ellen i = 0; i < c.mRows * c.mCols; i++) {
		o[i] = c.mEl[i] + n;
	}
	return CTM<T>::CTM(o, o + c.mRows * c.mCols, c.mRows, c.mCols);
}

template <class T>
CTM<T> operator-(const T & n, const CTM<T> & c) {
	typename CTM<T>::element o = new T[c.mRows * c.mCols];
	for(typename CTM<T>::ellen i = 0; i < c.mRows * c.mCols; i++) {
		if(std::isinf(o[i] = n - c.mEl[i])) {
			std::cout << "Divide by zero at [" << c.mRows << " " << c.mCols << "]. Setting element to zero." << std::endl;
			o[i] = 0;
		}
	}
	return CTM<T>::CTM(o, o + c.mRows * c.mCols, c.mRows, c.mCols);
}

template <class T>
CTM<T> operator*(const T & n, const CTM<T> & c) {
	typename CTM<T>::element o = new T[c.mRows * c.mCols];
	for(typename CTM<T>::ellen i = 0; i < c.mRows * c.mCols; i++) {
		o[i] = c.mEl[i] * n;
	}
	return CTM<T>::CTM(o, o + c.mRows * c.mCols, c.mRows, c.mCols);
}

template <class T>
CTM<T> operator/(const T & n, const CTM<T> & c) {
	typename CTM<T>::element o = new T[c.mRows * c.mCols];
	for(typename CTM<T>::ellen i = 0; i < c.mRows * c.mCols; i++) {
		o[i] = n / c.mEl[i];
	}
	return CTM<T>::CTM(o, o + c.mRows * c.mCols, c.mRows, c.mCols);
}

// Comparison

template <class T>
bool CTM<T>::operator==(const CTM<T> & c) {
	if(mRows != c.mRows || mCols != c.mCols)
		return false;
	
	for(ellen i = 0; i < mRows * mCols; i++) {
		if(mEl[i] != c.mEl[i])
			return false;
	}
	return true;
}

// Printing

template <class T>
void CTM<T>::show() const {
	for(uns i = 0; i < mRows; i++) {
		std::cout << "\n";
		for(uns j = 0; j < mCols-1; j++) {
			std::cout << mEl[point(i, j)] << " ";
		}
		std::cout << mEl[point(i, mCols-1)];
	}
	std::cout << std::endl;
}

}

#endif
