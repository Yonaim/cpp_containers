// https://stl.boost.org/select1st.html
#ifndef FT_SELECT1ST_H
#define FT_SELECT1ST_H

#include "unary_function.h"

/**
 * @brief 한 pair를 받아 pair의 첫번째 요소를 반환
 */

namespace ft
{
    template <typename Pair>
    struct Select1st : public unary_function<Pair, typename Pair::first_type>
    {
        const typename Pair::first_type &operator()(const Pair &p) const { return p.first; }
    };
} // namespace ft

#endif
