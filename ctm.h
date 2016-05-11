#ifndef CTM_H_
#define CTM_H_

#include <climits>
#include <iostream>

namespace ct {
	template <class T>
	class CTM {
	public:
		typedef unsigned uns;
		typedef unsigned long ellen; // element length type
		// Non-const Iterator Typedefs
		typedef T* start_iterator;
		typedef T* end_iterator;
		typedef T* start_col_iterator;
		typedef T* end_col_iterator;
		typedef T* start_row_iterator;
		typedef T* end_row_iterator;
		// Const Iterator Typedefs
		typedef const T* const_start_iterator;
		typedef const T* const_end_iterator;
		typedef const T* const_start_col_iterator;
		typedef const T* const_end_col_iterator;
		typedef const T* const_start_row_iterator;
		typedef const T* const_end_row_iterator;
	private:
		T* el;
		unsigned nrow;
		unsigned ncol;
	public:
		// Constructor
		CTM<T>(T repnum=0, uns rows=1, uns cols=1);
		CTM<T>(const_start_iterator s, const_end_iterator e, uns rows, uns cols);
		CTM<T>(const_start_row_iterator sr, const_end_row_iterator er, const_start_col_iterator sc, const_end_col_iterator ec);
		// Destructor
		virtual ~CTM<T>();
		// Attributes
		const uns & nrows();
		const uns & ncols();
		// Scalar
		const bool isScalar() const;
		const T scalar() const;
		// Reshape
		void reshape(uns new_nrows, uns new_ncols);
		// Concatenation 0=rows, 1=cols
		void concat(const CTM<T1> & c1, axis=0);
		CTM<T> concat(const CTM<T1> & c1, axis=0) const;
		// Iterators
		start_iterator begin();
		end_iterator end();
		start_row_iterator row_begin(uns i);
		end_row_iterator row_end(uns i);
		start_col_iterator col_begin(uns j);
		end_col_iterator col_end(uns j);
		// Const Iterators
		const_start_iterator cbegin();
		const_end_iterator cend();
		const_start_row_iterator crow_begin(uns i);
		const_end_row_iterator crow_end(uns i);
		const_start_col_iterator ccol_begin(uns j);
		const_end_col_iterator ccol_end(uns j);
		// Const Element Access
		virtual const T & at(const unsigned & i, const unsigned & j) const;
		// Non-Const Element Access
		virtual T & at(const uns & i, const uns & j, const T & n);
		// Transpose
		virtual start_row_iterator t() const;
		virtual const_start_row_iterator ct() const; // const
		// Subset Includes Endpoint
		const CTM<T> subset(unsigned startrow, unsigned endrow, unsigned startcol, unsigned endcol);
		// Matrix Operations
		virtual const CTM<T> operator+(const CTM<T1> & c1) const;
		virtual const CTM<T> operator-(const CTM<T1> & c1) const;
		virtual const CTM<T> operator*(const CTM<T1> & c1) const;
		// Transpose Matrix Operations
		virtual const CTM<T> operator+(const_start_row_iterator csri) const;
		virtual const CTM<T> operator-(const_start_row_iterator csri) const;
		virtual const CTM<T> operator*(const_start_row_iterator csri) const;
		// Friend Matrix Operations Element-wise
		const CTM<T> operator+(T1, const CTM<T> & c0);
		const CTM<T> operator-(T1, const CTM<T> & c0);
		const CTM<T> operator*(T1, const CTM<T> & c0);
		const CTM<T> operator/(T1, const CTM<T> & c0);
		// Type Conversions
		const CTM<double> dConv() const;
		const CTM<int> intConv() const;
		const CTM<long> lConv() const;
		const CTM<long long> llConv() const;
		// Friends
		friend const CTM<T> transpose(const CTM<T> & c0);
		friend ostream & operator<<(ostream & os);
	};
};

#endif