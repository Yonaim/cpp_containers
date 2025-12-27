#ifndef FT_ITERATOR_TAGS_H
#define FT_ITERATOR_TAGS_H

namespace ft
{
    struct input_iterator_tag
    {
    };

    struct output_iterator_tag
    {
    };

    struct forward_iterator_tag : input_iterator_tag
    {
    };

    struct bidirectional_iterator_tag : forward_iterator_tag
    {
    };

    struct random_access_iterator_tag : bidirectional_iterator_tag
    {
    };

    struct contiguous_iterator_tag : random_access_iterator_tag
    {
    };
} // namespace ft

#endif
