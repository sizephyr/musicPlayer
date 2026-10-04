#pragma once

/**
 * \brief Шаблон для простого создания синглтона по Мейерсу (CRTP)
 *
 * \example \code
class MySingl: public Singleton<MySingl> {
    friend class Singleton<MySingl>;
    MySingl() = default; // private constructor
public:
    void func();
};
...
void use_mysingl(){
    MySingl::i().func();
}
 * \endcode
 */
template <typename T>
class Singleton {
 protected:
  Singleton() = default;

 public:
  static T& i() {
    static T instance;
    return instance;
  }
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;
  // Виртуальный деструктор не нужен, класс используется как CRTP
  ~Singleton() = default;
};