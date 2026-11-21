#ifndef PARTITION_H
#define PARTITION_H

#include <iterator>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>

#include <wx/textctrl.h>

using namespace std;

class Partition
{
private:
	wxTextCtrl *listing;
	ostringstream line;

	void ouputPartitionLine();

public:

	Partition (wxTextCtrl *tctrl);
	~Partition()
	{}

	void listAllPartitions (int n);
	void listPartitionsSizeK (int n, int k);

	class iterator
		: public std::iterator<std::bidirectional_iterator_tag,
		  const std::vector<unsigned> >
	{
	public:
		iterator (unsigned n, bool first = true);

		reference operator*()  const
		{
			return kappa;
		}
		pointer   operator->() const
		{
			return &kappa;
		}

		unsigned  size() const
		{
			return kappa.size();
		}
		unsigned  subsets() const
		{
			return M[size() - 1] + 1;
		}

		iterator& operator++();
		iterator& operator--();

		template <typename T>
		std::auto_ptr<std::vector<std::vector<T> > >
		operator[] (const std::vector<T> &v) const;

	protected:
		std::vector<unsigned> kappa, M;

		void integrityCheck();
	};

	class iterator_k: public iterator
	{
	public:
		iterator_k (unsigned n, unsigned psize, bool first = true);

		// optimized version
		unsigned subsets() const
		{
			return psize;
		}

		iterator_k& operator++();
		iterator_k& operator--();

	private:
		const unsigned psize;

		void integrityCheck();
	};
};

extern std::ostream& operator<< (std::ostream& out,
                                 Partition::iterator& it);

template <typename T>
std::auto_ptr<std::vector<std::vector<T> > >
Partition::iterator::operator[] (const std::vector<T> &v) const
{
	std::vector<std::vector<T> > *part = new std::vector<std::vector<T> > (subsets());

	for (unsigned i = 0; i < size(); ++i)
		(*part) [kappa[i]].push_back (v[i]);

	return std::auto_ptr<std::vector<std::vector<T> > > (part);
}

template <typename T>
std::ostream& operator<< (std::ostream& out, std::vector<T> &s)
{
	out << '{';

	for (unsigned i = 0; i < s.size() - 1; ++i)
		out << s[i] << "  ";

	out << * (s.end()-1) << '}';

	return out;
}

template <typename T>
std::ostream& operator<< (std::ostream& out, std::vector<std::vector<T> >& part)
{
	for (unsigned i = 0; i < part.size() - 1; ++i)
		out << part[i] << ' ';

	out << * (part.end()-1);

	return out;
}

#endif
