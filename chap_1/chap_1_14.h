template<typename Sinature>
class SingleTrivial;

// NOT thread safe !!!
template <typename RET, typename... ARGS>
class SingleTrivial<RET(ARGS...)>
{
public:
    typedef std::function<void (ARGS...)> Functor;

    void connect(Functor&& func)
    {
        functors_.push_back(std::forward<Functor>(func));
    }

    void call(ARGS&&... args)
    {
        for (const Functor& f: functors_)
        {
            f(args);
        }
    }

private:
    std::vector<Functor> functors_;
};