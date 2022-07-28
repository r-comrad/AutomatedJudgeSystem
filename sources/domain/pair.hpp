#ifndef DOM_PAIR_HPP
#define DOM_PAIR_HPP

//--------------------------------------------------------------------------------

namespace dom
{
    template<typename T1, typename T2 = T1>
    union Pair
    {
        struct
        {
            T1 i;
            T2 j;
        };
        struct
        {
            T1 x;
            T2 y;
        };
        struct
        {
            T1 key;
            T2 value;
        };
        struct
        {
            T1 first;
            T2 second;
        };

        Pair() noexcept :
            x(),
            y()
        {}

        template <typename ARG>
        Pair(ARG&& other) noexcept : 
            x(std::forward<T1>(other.x)),
            y(std::forward<T2>(other.y))
        {}

        template <typename ARG1, typename ARG2>
        Pair(ARG1&& xInit, ARG2&& yInit) noexcept :
            x(std::forward<T1>(xInit)),
            y(std::forward<T2>(yInit))
        {}

        // not default by purpose 
        ~Pair(){}

        void operator+= (const Pair& other) noexcept
        {
            x += other.x;
            y += other.y;
        }
        void operator-= (const Pair& other) noexcept
        {
            x -= other.x;
            y -= other.y;
        }

        template <typename ARG>
        Pair& operator=(ARG&& other) noexcept
        {
            x = std::forward<T1>(other.x);
            y = std::forward<T2>(other.y);
            return *this;
        }

        friend Pair operator+ (const Pair& a1, const Pair& a2) noexcept
        {
            Pair<T1, T2> res{ a1.x + a2.x, a1.y + a2.y };
            return res;
        }
        friend Pair operator+ (const Pair& a1, Pair&& a2) noexcept
        {
            a2.x += a1.x;
            a2.y += a1.y;
            return a2;
        }
        friend Pair operator+ (Pair&& a1, const Pair& a2) noexcept
        {
            return a2 + std::move(a1);
        }
        friend Pair operator+ (Pair&& a1, Pair&& a2) noexcept
        {
            a1.x += a2.x;
            a1.y += a2.y;
            return a1;
        }

        friend Pair operator- (const Pair& a1, const Pair& a2) noexcept
        {
            Pair<T1, T2> res{ a1.x - a2.x, a1.y - a2.y };
            return res;
        }
        friend Pair operator- (const Pair& a1, Pair&& a2) noexcept
        {
            a2.x -= a1.x;
            a2.y -= a1.y;
            return a2;
        }
        friend Pair operator- (Pair&& a1, const Pair& a2) noexcept
        {
            return a2 - std::move(a1);
        }
        friend Pair operator- (Pair&& a1, Pair&& a2) noexcept
        {
            a1.x -= a2.x;
            a1.y -= a2.y;
            return a1;
        }

        bool operator< (const Pair& other) const noexcept
        {
            if (x != other.x) return y < other.y;
            return y < other.y;
        }
        bool operator>(const Pair& other) const noexcept
        {
            if (x != other.x) return y > other.y;
            return y > other.y;
        }
        bool operator== (const Pair& other) const noexcept
        {
            return x == other.x && y == other.y;
        }
        bool operator!= (const Pair& other) const noexcept
        {
            return x != other.x || y != other.y;
        }

        private:
        class PairBracketAccessResult
        {
            friend Pair;
        public:
            operator T1&() noexcept
            {
                #ifdef _DBG_
                if (mIndex != 0) exit(-1);
                #endif

                return mRes.x;
            }

            operator T2&() noexcept
            {
                #ifdef _DBG_
                if (mIndex != 1) exit(-1);
                #endif

                return mRes.y;
            }

        private:
            Pair& mRes;
            int mIndex;

            PairBracketAccessResult(Pair& aRes, int aIndex) noexcept :
                mRes    (aRes),
                mIndex    (aIndex)
            {}
        };

        public:
            PairBracketAccessResult operator[] (const int& aIndex) noexcept
            {
                return PairBracketAccessResult(*this, aIndex);
            }
    };
}

//--------------------------------------------------------------------------------

#endif // !DOM_PAIR_HPP