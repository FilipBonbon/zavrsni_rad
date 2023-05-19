#ifndef ZAVRSNI_RAD_PARENTSELECTION_HPP
#define ZAVRSNI_RAD_PARENTSELECTION_HPP

class ParentSelection {
public:
    virtual ~ParentSelection() = default;

    virtual int * select(int **, double *, int, int) = 0;
};

#endif
