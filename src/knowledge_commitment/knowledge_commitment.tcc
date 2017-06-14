/** @file
 *****************************************************************************

 Implementation of interfaces for:
 - a knowledge commitment, and
 - a knowledge commitment vector.

 See knowledge_commitment.hpp .

 *****************************************************************************
 * @author     This file is part of libsnark, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef KNOWLEDGE_COMMITMENT_TCC_
#define KNOWLEDGE_COMMITMENT_TCC_

namespace libsnark {

template<typename T1, typename T2>
knowledge_commitment<T1,T2>::knowledge_commitment(const T1 &g, const T2 &h) :
    g(g), h(h)
{
}

template<typename T1, typename T2>
knowledge_commitment<T1,T2> knowledge_commitment<T1,T2>::zero()
{
    return knowledge_commitment<T1,T2>(T1::zero(), T2::zero());
}

template<typename T1, typename T2>
knowledge_commitment<T1,T2> knowledge_commitment<T1,T2>::one()
{
    return knowledge_commitment<T1,T2>(T1::one(), T2::one());
}

template<typename T1, typename T2>
knowledge_commitment<T1,T2> knowledge_commitment<T1,T2>::operator+(const knowledge_commitment<T1,T2> &other) const
{
    return knowledge_commitment<T1,T2>(this->g + other.g,
                                       this->h + other.h);
}

template<typename T1, typename T2>
bool knowledge_commitment<T1,T2>::is_zero() const
{
    return (g.is_zero() && h.is_zero());
}

template<typename T1, typename T2>
bool knowledge_commitment<T1,T2>::operator==(const knowledge_commitment<T1,T2> &other) const
{
    return (this->g == other.g &&
            this->h == other.h);
}

template<typename T1, typename T2>
bool knowledge_commitment<T1,T2>::operator!=(const knowledge_commitment<T1,T2> &other) const
{
    return !((*this) == other);
}

template<typename T1, typename T2>
void knowledge_commitment<T1,T2>::to_special()
{
    this->g.to_special();
    this->h.to_special();
}

template<typename T1, typename T2>
bool knowledge_commitment<T1,T2>::is_special() const
{
    return this->g->is_special() && this->h->is_special();
}

template<typename T1, typename T2>
knowledge_commitment<T1,T2> knowledge_commitment<T1,T2>::mixed_add(
    const knowledge_commitment<T1,T2> &other) const
{
    return knowledge_commitment<T1,T2>(this->g.mixed_add(other.g),
                                       this->h.mixed_add(other.h));
}

template<typename T1, typename T2>
knowledge_commitment<T1,T2> knowledge_commitment<T1,T2>::dbl() const
{
    return knowledge_commitment<T1,T2>(this->g.dbl(), this->h.dbl());
}

template<typename T1, typename T2, mp_size_t m>
knowledge_commitment<T1,T2> operator*(const libff::bigint<m> &lhs, const knowledge_commitment<T1,T2> &rhs)
{
    return knowledge_commitment<T1,T2>(lhs * rhs.g,
                                       lhs * rhs.h);
}

template<typename T1, typename T2, mp_size_t m, const libff::bigint<m> &modulus_p>
knowledge_commitment<T1,T2> operator*(const libff::Fp_model<m, modulus_p> &lhs, const knowledge_commitment<T1,T2> &rhs)
{
    return (lhs.as_bigint()) * rhs;
}

template<typename T1, typename T2>
void knowledge_commitment<T1,T2>::print() const
{
    printf("knowledge_commitment.g:\n");
    g.print();
    printf("knowledge_commitment.h:\n");
    h.print();
}

template<typename T1, typename T2>
size_t knowledge_commitment<T1,T2>::size_in_bits()
{
        return T1::size_in_bits() + T2::size_in_bits();
}

template<typename T1,typename T2>
std::ostream& operator<<(std::ostream& out, const knowledge_commitment<T1,T2> &kc)
{
    out << kc.g << OUTPUT_SEPARATOR << kc.h;
    return out;
}

template<typename T1,typename T2>
std::istream& operator>>(std::istream& in, knowledge_commitment<T1,T2> &kc)
{
    in >> kc.g;
    libff::consume_OUTPUT_SEPARATOR(in);
    in >> kc.h;
    return in;
}

template<typename T1, typename T2>
void knowledge_commitment<T1,T2>::batch_to_special_all_non_zeros(
    std::vector<knowledge_commitment<T1,T2> > &vec)
{
    std::vector<T1> g_special;
    std::vector<T2> h_special;
    g_special.reserve(vec.size());
    h_special.reserve(vec.size());

    for (size_t i = 0; i < vec.size(); i++)
    {
        g_special.emplace_back(vec[i].g);
        h_special.emplace_back(vec[i].h);
    }

    T1::batch_to_special_all_non_zeros(g_special);
    T2::batch_to_special_all_non_zeros(h_special);

    for (size_t i = 0; i < vec.size(); i++)
    {
        vec[i].g = g_special[i];
        vec[i].h = h_special[i];
    }
}

} // libsnark

#endif // KNOWLEDGE_COMMITMENT_TCC_
