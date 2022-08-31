#include "Velox/Interpreter/VeloxImage.h"

#include <Velox/Interpreter/ObjectType.h>
#include <Velox/Interpreter/Item.h>
#include <Velox/Interpreter/ItemStateFunction.h>
#include <Velox/Interpreter/ItemStateInteger.h>
#include <Velox/Interpreter/InterpreterFunction.h>
#include <Velox/Interpreter/InterpreterFunctionOneParameter.h>
#include <Velox/Interpreter/InterpreterError.h>
#include <AstuGraphics.h>

#include <cassert>
#include <functional>

using namespace velox;
using namespace astu;
using namespace std;

#define TYPE_NAME "Image"

//using ImageData = DataWrapper<std::shared_ptr<Image>>;

class ImageData : public Image, public ItemData {
public:
    ImageData(int width, int height) : Image(width, height) {}
    ImageData(const Image& image) : Image(image.GetWidth(), image.GetHeight()){
        for (size_t i = 0; i < image.NumberOfPixels(); ++i) {
            SetPixel(i, image.GetPixel(i));
        }
    }
};

class ImageConstructor : public InterpreterFunction {
public:

    ImageConstructor() {
        AddFormalParameter("width");
        AddFormalParameter("height");
    }

protected:

    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        const int width = sc.GetItem("width").GetIntegerValue();
        const int height = sc.GetItem("height").GetIntegerValue();
        auto item = sc.FindItem("this");
        assert(item);
        item->SetData(make_shared<ImageData>(width, height));
        return item;
    }

};

class ImageFunctionNoParam : public InterpreterFunction {
public:

    ImageFunctionNoParam(std::function<std::shared_ptr<Item> (Image&)> func) : func(func) {}

protected:

    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        std::shared_ptr<Image> image = std::dynamic_pointer_cast<ImageData>(sc.FindItem("this")->GetData());
        return func(*image);
    }

private:
    std::function<std::shared_ptr<Item> (Image&)> func;
};

class ImageFunctionOneParam : public InterpreterFunction {
public:

    ImageFunctionOneParam(std::function<std::shared_ptr<Item> (Image&, Item&)> func) : func(func) {
        AddFormalParameter("a");
    }

protected:

    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        std::shared_ptr<Image> image = std::dynamic_pointer_cast<ImageData>(sc.FindItem("this")->GetData());
        return func(*image, sc.GetItem("a"));
    }

private:
    std::function<std::shared_ptr<Item> (Image&, Item&)> func;
};

class ImageFunctionTwoParam : public InterpreterFunction {
public:

    ImageFunctionTwoParam(std::function<std::shared_ptr<Item> (Image&, Item& a, Item& b)> func) : func(func) {
        AddFormalParameter("a");
        AddFormalParameter("b");
    }

protected:

    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        std::shared_ptr<Image> image = std::dynamic_pointer_cast<ImageData>(sc.FindItem("this")->GetData());
        return func(*image, sc.GetItem("a"), sc.GetItem("b"));
    }

private:
    std::function<std::shared_ptr<Item> (Image&, Item& a, Item& b)> func;
};

class ImageFunctionThreeParam : public InterpreterFunction {
public:

    ImageFunctionThreeParam(std::function<std::shared_ptr<Item> (Image&, Item& a, Item& b, Item& c)> func) : func(func) {
        AddFormalParameter("a");
        AddFormalParameter("b");
        AddFormalParameter("c");
    }

protected:

    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        std::shared_ptr<Image> image = std::dynamic_pointer_cast<ImageData>(sc.FindItem("this")->GetData());
        return func(*image, sc.GetItem("a"), sc.GetItem("b"), sc.GetItem("c"));
    }

private:
    std::function<std::shared_ptr<Item> (Image&, Item& a, Item& b, Item& c)> func;
};

class ImageFunctionFourParam : public InterpreterFunction {
public:

    ImageFunctionFourParam(std::function<std::shared_ptr<Item> (Image&, Item& a, Item& b, Item& c, Item& d)> func) : func(func) {
        AddFormalParameter("a");
        AddFormalParameter("b");
        AddFormalParameter("c");
        AddFormalParameter("d");
    }

protected:

    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        std::shared_ptr<Image> image = std::dynamic_pointer_cast<ImageData>(sc.FindItem("this")->GetData());
        return func(*image, sc.GetItem("a"), sc.GetItem("b"), sc.GetItem("c"), sc.GetItem("d"));
    }

private:
    std::function<std::shared_ptr<Item> (Image&, Item& a, Item& b, Item& c, Item& d)> func;
};


class ImageFunctionSetPixel : public InterpreterFunction {
public:

    ImageFunctionSetPixel() {
        AddFormalParameter("x");
        AddFormalParameter("y");
        AddFormalParameter("color");
    }

protected:

    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        std::shared_ptr<Image> image = std::dynamic_pointer_cast<ImageData>(sc.FindItem("this")->GetData());
        int x = sc.GetItem("x").GetIntegerValue();
        int y = sc.GetItem("y").GetIntegerValue();
        auto color = sc.GetItem("color").GetColorValue();

        image->SetPixel(
                sc.GetItem("x").GetIntegerValue(),
                sc.GetItem("y").GetIntegerValue(),
                sc.GetItem("color").GetColorValue());

        return Item::CreateUndefined();
    }

private:
    std::function<std::shared_ptr<Item> (Image&, Item& a, Item& b, Item& c)> func;
};

class ImageFunctionBlend : public InterpreterFunction {
public:

    ImageFunctionBlend() {
        AddFormalParameter("img");
        AddFormalParameter("sx");
        AddFormalParameter("sy");
        AddFormalParameter("sw");
        AddFormalParameter("sh");
        AddFormalParameter("dx");
        AddFormalParameter("dy");
        AddFormalParameter("tint");
    }

protected:

    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        std::shared_ptr<Image> image = std::dynamic_pointer_cast<ImageData>(sc.FindItem("this")->GetData());
        auto img = dynamic_pointer_cast<Image>(sc.GetItem("img").GetData());
        if (!img) {
            throw InterpreterError("first parameter of image.Blend must be of type image");
        }

        int sx = sc.GetItem("sx").GetIntegerValue();
        int sy = sc.GetItem("sy").GetIntegerValue();
        int sw = sc.GetItem("sw").GetIntegerValue();
        int sh = sc.GetItem("sh").GetIntegerValue();
        int dx = sc.GetItem("dx").GetIntegerValue();
        int dy = sc.GetItem("dy").GetIntegerValue();

        auto &tint = sc.GetItem("tint");
        if (tint.IsUndefined()) {
            image->Blend(*img, sx, sy, sw, sh, dx, dy);
        } else {
            image->Blend(*img, sx, sy, sw, sh, dx, dy, tint.GetColorValue());
        }

        return Item::CreateUndefined();
    }

private:
    std::function<std::shared_ptr<Item> (Image&, Item& a, Item& b, Item& c)> func;
};

class ImageFunctionGetAvgRegionColor : public InterpreterFunction {
public:

    ImageFunctionGetAvgRegionColor() {
        AddFormalParameter("x");
        AddFormalParameter("y");
        AddFormalParameter("w");
        AddFormalParameter("h");
    }

protected:

    std::shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        std::shared_ptr<Image> image = std::dynamic_pointer_cast<ImageData>(sc.FindItem("this")->GetData());

        int x = sc.GetItem("x").GetIntegerValue();
        int y = sc.GetItem("y").GetIntegerValue();
        int w = sc.GetItem("w").GetIntegerValue();
        int h = sc.GetItem("h").GetIntegerValue();

        return Item::CreateColor(image->GetAverageRegionColor(x, y, w, h));
    }

private:
    std::function<std::shared_ptr<Item> (Image&, Item& a, Item& b, Item& c)> func;
};


class ImageExporter : public InterpreterFunction {
public:

    ImageExporter() {
        AddFormalParameter("image");
        AddFormalParameter("filename");
    }

private:
    shared_ptr<Item> DoEvaluate(ScriptContext &sc, unsigned int lineNumber) override {
        auto image = std::dynamic_pointer_cast<ImageData>(sc.GetItem("image").GetData());
        if (!image) {
            throw InterpreterError("first parameter must be of type image", lineNumber);
        }

        StoreImage(*image, sc.GetItem("filename").GetStringValue(sc));
        return Item::CreateUndefined();
    }
};

void AddImageSupport(Interpreter &interpreter) {
    auto imageType = make_shared<ObjectType>();

    imageType->AddItem(TYPE_NAME, Item::CreateFunction(make_shared<ImageConstructor>()));

    imageType->AddItem("ConvertToGrayscale", Item::Create(make_unique<ItemStateFunction>(
            make_shared<ImageFunctionNoParam>([](Image &img) -> std::shared_ptr<Item> {
                img.ConvertToGrayscale();
                return Item::CreateUndefined();
            }))));

    imageType->AddItem("GetWidth", Item::Create(make_unique<ItemStateFunction>(
            make_shared<ImageFunctionNoParam>([](Image &img) -> std::shared_ptr<Item> {
                return Item::Create(make_unique<ItemStateInteger>(img.GetWidth()));
            }))));

    imageType->AddItem("GetHeight", Item::CreateFunction(
            make_shared<ImageFunctionNoParam>([](Image &img) -> std::shared_ptr<Item> {
                return Item::Create(make_unique<ItemStateInteger>(img.GetHeight()));
            })));

    imageType->AddItem("GetAspectRatio",
                       Item::CreateFunction(make_shared<ImageFunctionNoParam>([](Image &img) -> std::shared_ptr<Item> {
                           return Item::CreateReal(img.GetAspectRatio());
                       })));

    imageType->AddItem("SetPixel", Item::CreateFunction(make_shared<ImageFunctionSetPixel>()));
    imageType->AddItem("Blend", Item::CreateFunction(make_shared<ImageFunctionBlend>()));
    imageType->AddItem("GetAverageRegionColor", Item::CreateFunction(make_shared<ImageFunctionGetAvgRegionColor>()));

    imageType->AddItem("GetAverageRegionBrightness", Item::CreateFunction(
            make_shared<ImageFunctionFourParam>([](Image &img, Item &a, Item &b, Item &c, Item &d) -> shared_ptr<Item> {
                double br = img.GetAverageRegionBrightness(a.GetIntegerValue(), b.GetIntegerValue(),
                                                           c.GetIntegerValue(), d.GetIntegerValue());
                return Item::CreateReal(br);
            })));

    imageType->AddItem("GetPixel", Item::CreateFunction(
            make_shared<ImageFunctionTwoParam>([](Image &img, Item &a, Item &b) -> shared_ptr<Item> {
                return Item::CreateColor(img.GetPixel(a.GetIntegerValue(), b.GetIntegerValue()));
            })));


    imageType->AddItem("Clear", Item::CreateFunction(
            make_shared<ImageFunctionOneParam>([](Image &img, Item &a) -> shared_ptr<Item> {
                if (a.IsUndefined()) {
                    img.Clear(Color4d(0, 0, 0));
                } else {
                    img.Clear(a.GetColorValue());
                }
                return Item::CreateUndefined();
            })));

    interpreter.AddObjectType(TYPE_NAME, imageType);
    interpreter.AddFunction("StoreImage", make_shared<ImageExporter>());
    interpreter.AddFunction("LoadImage", make_shared<InterpreterFunctionOneParameter>(
            [](ScriptContext &sc, std::shared_ptr<Item> param, unsigned int lineNumber) -> std::shared_ptr<Item> {
                try {
                    auto image = LoadImage(param->GetStringValue(sc));
                    auto item = sc.FindObjectType(TYPE_NAME)->CreateObject(sc);
                    item->SetData(make_shared<ImageData>(*image));
                    return item;
                } catch (std::runtime_error &e) {
                    throw InterpreterError(e.what(), lineNumber);
                }
            }));
}
