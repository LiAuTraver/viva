#pragma once

#ifdef print
#warning "print(x) is already defined, undefining it."
#undef print(x)
#endif

#ifdef println
#warning "println(...) is already defined, undefining it."
#undef println(...)
#endif

#ifdef fprint
#warning "fprint(x) is already defined, undefining it."
#undef fprint(x)
#endif

#ifdef fprintln
#warning "fprintln(x) is already defined, undefining it."
#undef fprintln(x)
#endif

#ifdef is_string
#warning "is_string(x) is already defined, undefining it."
#undef is_string(x)
#endif

#ifdef get_raw
#warning "get_raw(...) is already defined, undefining it."
#undef get_raw(...)
#endif

#ifdef get_rec
#warning "get_rec(...) is already defined, undefining it."
#undef get_rec(...)
#endif

#ifdef smart
#warning "smart is already defined, undefining it."
#undef smart
#endif

#ifdef alloc
#warning "alloc is already defined, undefining it."
#undef alloc
#endif
