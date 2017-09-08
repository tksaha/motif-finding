#include "graph.h"

//complexity analysis: http://www.drdobbs.com/the-boost-multi-index-containers-library/184401843
int  id_record_length(const heapData &d)
{
        return d.count;
}


typedef multi_index_container<
  heapData,
  indexed_by<
    ordered_unique<
      tag<codex>,  BOOST_MULTI_INDEX_MEMBER(heapData ,string,code)
    >,
    ordered_non_unique<
    tag<graphidsx>,global_fun<const heapData&, int , id_record_length>
    >
  >
> code_set;


template<typename Tag,typename MultiIndexContainer>
void print_out_by(
 const MultiIndexContainer& s
// Tag* =0 /* fixes a MSVC++ 6.0 bug with implicit template function parms */
)
{
  /* obtain a reference to the index tagged by Tag */

  const typename boost::multi_index::index<MultiIndexContainer,Tag>::type& i=
    get<Tag>(s);

  typedef typename MultiIndexContainer::value_type value_type;

  /* dump the elements of the index to cout */

  std::copy(i.begin(),i.end(),std::ostream_iterator<value_type>(std::cout));
}

code_set mycodeset;
