#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#ifndef NAMESPACE
#define NAMESPACE ft
#endif

#if defined(NAMESPACE) && NAMESPACE == ft
# define NAMESPACE_NAME "ft"
#else
# define NAMESPACE_NAME "std"
#endif

#endif // TEST_CONFIG_H
