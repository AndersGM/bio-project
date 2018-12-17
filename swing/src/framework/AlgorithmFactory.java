package framework;

public interface AlgorithmFactory {

    TextPresenter createTextPresenter();

    Search createSearch();

    PreProcess createPipeline();
}
