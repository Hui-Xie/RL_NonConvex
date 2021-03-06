
#include <Optimizer.h>

#include "Optimizer.h"
#include "TensorBlas.h"

Optimizer::Optimizer(const float lr) {
   m_lr = lr;
   m_type = "Optimizer";
}

Optimizer::~Optimizer() {

}

void Optimizer::setLearningRate(const float lr) {
   m_lr = lr;
}

float Optimizer::getLearningRate() {
   return m_lr;
}


SGDOptimizer::SGDOptimizer(const float lr) : Optimizer(lr) {
    m_type = "SGD";
}

SGDOptimizer::~SGDOptimizer() {

}

void SGDOptimizer::sgd(const Tensor<float> *pG, Tensor<float> *pW) {
    axpy(-m_lr, pG, pW);
}


AdamOptimizer::AdamOptimizer(const float lr, const float beta1, const float beta2) : Optimizer(lr) {
   m_beta1 = beta1;
   m_beta2 = beta2;
   m_epsilon = 1e-8;
   m_t = 1;
   m_type = "Adam";
}

AdamOptimizer::~AdamOptimizer() {

}

void AdamOptimizer::adam(Tensor<float> *pM, Tensor<float> *pR, const Tensor<float> *pG, Tensor<float> *pW) {
    const int t = m_t;
    matAdd(m_beta1, pM, 1-m_beta1, pG, pM);

    Tensor<float> G2(pG->getDims());
    int N = pG->getLength();
    vsMul(N, pG->getData(), pG->getData(), G2.getData());
    matAdd(m_beta2, pR, 1-m_beta2, &G2, pR);

    float a = m_lr * sqrt(1-pow(m_beta2, t))/(1- pow(m_beta1, t));

    Tensor<float> RSqrt(pR->getDims());
    N = RSqrt.getLength();
    vsSqrt(N, pR->getData(), RSqrt.getData());

    Tensor<float> deltaW(pR->getDims());
    vsLinearFrac(N, pM->getData(), RSqrt.getData(), -a, 0, 1, m_epsilon, deltaW.getData());
    axpy(1.0, &deltaW, pW);
}

void AdamOptimizer::setIteration(const int t) {
    m_t = t;
}

int AdamOptimizer::getIteration() {
    return m_t;
}

void AdamOptimizer::increaseIteration() {
   ++m_t;
}



