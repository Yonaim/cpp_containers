#ifndef FT_MAP_H
#define FT_MAP_H

#include <cstddef>
#include <functional>
#include "pair.h"

namespace ft
{

    template <class Key, class T, class Compare = std::less<Key> >
    class map
    {
      public:
        // ---------------- Member types ----------------
        typedef Key                    key_type;
        typedef T                      mapped_type;
        typedef ft::pair<const Key, T> value_type;
        typedef Compare                key_compare;
        typedef std::size_t            size_type;

      private:
        // ---------------- Node definition ----------------
        struct Node
        {
            value_type value;
            Node      *parent;
            Node      *left;
            Node      *right;
            bool       red;

            Node(const value_type &v) : value(v), parent(0), left(0), right(0), red(true) {}
        };

      public:
        // ---------------- value_compare ----------------
        class value_compare
        {
            friend class map;

          protected:
            Compare comp;
            value_compare(Compare c) : comp(c) {}

          public:
            bool operator()(const value_type &lhs, const value_type &rhs) const
            {
                return comp(lhs.first, rhs.first);
            }
        };

        // ---------------- iterator ----------------
        class iterator
        {
          public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef std::ptrdiff_t                  difference_type;
            typedef typename map::value_type        value_type;
            typedef value_type                     *pointer;
            typedef value_type                     &reference;

            iterator() : n_(0) {}
            explicit iterator(Node *n) : n_(n) {}

            reference operator*() const { return n_->value; }
            pointer   operator->() const { return &n_->value; }

            iterator &operator++() { return *this; }
            iterator  operator++(int)
            {
                iterator tmp(*this);
                return tmp;
            }
            iterator &operator--() { return *this; }
            iterator  operator--(int)
            {
                iterator tmp(*this);
                return tmp;
            }

            friend bool operator==(const iterator &a, const iterator &b) { return a.n_ == b.n_; }
            friend bool operator!=(const iterator &a, const iterator &b) { return a.n_ != b.n_; }

            Node *base() const { return n_; }

          private:
            Node *n_;
        };

        // ---------------- const_iterator ----------------
        class const_iterator
        {
          public:
            typedef std::bidirectional_iterator_tag iterator_category;
            typedef std::ptrdiff_t                  difference_type;
            typedef typename map::value_type        value_type;
            typedef const value_type               *pointer;
            typedef const value_type               &reference;

            const_iterator() : n_(0) {}
            explicit const_iterator(Node *n) : n_(n) {}
            const_iterator(const iterator &it) : n_(it.base()) {}

            reference operator*() const { return n_->value; }
            pointer   operator->() const { return &n_->value; }

            const_iterator &operator++() { return *this; }
            const_iterator  operator++(int)
            {
                const_iterator tmp(*this);
                return tmp;
            }
            const_iterator &operator--() { return *this; }
            const_iterator  operator--(int)
            {
                const_iterator tmp(*this);
                return tmp;
            }

            friend bool operator==(const const_iterator &a, const const_iterator &b)
            {
                return a.n_ == b.n_;
            }
            friend bool operator!=(const const_iterator &a, const const_iterator &b)
            {
                return a.n_ != b.n_;
            }

          private:
            Node *n_;
        };

        // ---------------- Member type aliases ----------------
        typedef iterator       iterator_type;
        typedef const_iterator const_iterator_type;

      private:
        Node       *root_;
        size_type   size_;
        key_compare comp_;

      public:
        // ======================================================
        //                MEMBER FUNCTION PROTOTYPES
        // ======================================================

        // ----- Constructors / Destructor -----
        map() : root_(0), size_(0), comp_(Compare()) {}
        map(const map &other) : root_(0), size_(0), comp_(other.comp_) { (void)other; }
        ~map() {}
        map &operator=(const map &other)
        {
            (void)other;
            return *this;
        }

        // ----- Element access -----
        T &operator[](const Key &key)
        {
            // dummy stub: insert if not exist
            (void)key;
            static T dummy;
            return dummy;
        }

        // ----- Iterators -----
        iterator       begin() { return iterator(root_); }
        iterator       end() { return iterator(0); }
        const_iterator begin() const { return const_iterator(root_); }
        const_iterator end() const { return const_iterator(0); }

        // ----- Capacity -----
        bool      empty() const { return size_ == 0; }
        size_type size() const { return size_; }
        size_type max_size() const { return static_cast<size_type>(-1) / sizeof(Node); }

        // ----- Modifiers -----
        ft::pair<iterator, bool> insert(const value_type &value)
        {
            (void)value;
            return ft::make_pair(iterator(root_), true);
        }

        iterator insert(iterator pos, const value_type &value)
        {
            (void)pos;
            (void)value;
            return iterator(root_);
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            (void)first;
            (void)last;
        }

        void      erase(iterator pos) { (void)pos; }
        size_type erase(const key_type &key)
        {
            (void)key;
            return 0;
        }
        void erase(iterator first, iterator last)
        {
            (void)first;
            (void)last;
        }

        void swap(map &other)
        {
            std::swap(root_, other.root_);
            std::swap(size_, other.size_);
        }

        void clear()
        {
            root_ = 0;
            size_ = 0;
        }

        // ----- Observers -----
        key_compare   key_comp() const { return comp_; }
        value_compare value_comp() const { return value_compare(comp_); }

        // ----- Operations -----
        iterator find(const key_type &key)
        {
            (void)key;
            return iterator(root_);
        }
        const_iterator find(const key_type &key) const
        {
            (void)key;
            return const_iterator(root_);
        }
        size_type count(const key_type &key) const
        {
            (void)key;
            return 0;
        }
        iterator lower_bound(const key_type &key)
        {
            (void)key;
            return iterator(root_);
        }
        const_iterator lower_bound(const key_type &key) const
        {
            (void)key;
            return const_iterator(root_);
        }
        iterator upper_bound(const key_type &key)
        {
            (void)key;
            return iterator(root_);
        }
        const_iterator upper_bound(const key_type &key) const
        {
            (void)key;
            return const_iterator(root_);
        }
        ft::pair<iterator, iterator> equal_range(const key_type &key)
        {
            (void)key;
            return ft::make_pair(iterator(root_), iterator(root_));
        }
        ft::pair<const_iterator, const_iterator> equal_range(const key_type &key) const
        {
            (void)key;
            return ft::make_pair(const_iterator(root_), const_iterator(root_));
        }

        // ----- Relational operators -----
        friend bool operator==(const map &lhs, const map &rhs) { return lhs.size_ == rhs.size_; }
        friend bool operator!=(const map &lhs, const map &rhs) { return !(lhs == rhs); }
        friend bool operator<(const map &lhs, const map &rhs) { return lhs.size_ < rhs.size_; }
        friend bool operator>(const map &lhs, const map &rhs) { return rhs < lhs; }
        friend bool operator<=(const map &lhs, const map &rhs) { return !(rhs < lhs); }
        friend bool operator>=(const map &lhs, const map &rhs) { return !(lhs < rhs); }
    };

} // namespace ft

#endif
