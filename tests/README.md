# react-native-firebase test suite

An **iOS** and **Android** React Native app built to test [`react-native-firebase`](https://github.com/invertase/react-native-firebase).

## Install

1. Clone the test application

```bash
git clone https://github.com/invertase/react-native-firebase.git
```

2. Install the dependencies listed in `package.json`

```bash
cd react-native-firebase/tests/ && npm install
```

### iOS Installation

3. Install the test application's CocoaPods.
* See [troubleshooting](#installing-podfiles) if this doesn't work for you.

```bash
npm run ios:pod:install
```

4. Start the React Native packager

```bash
npm run start
```

5. In another terminal window, install the app on your emulator:

```bash
npm run ios:dev
```

### Android Installation

6. Start your emulator through Android Studio: Tools > Android > AVD Manager

> You will need a version of the Android emulator that has the Play Store installed (you should be able to find it on the emulator's home screen or on the list of apps).

7. Start the React Native packager if you haven't already in the iOS instructions.

```bash
npm run start
```

8. Run the test app on your Android emulator:

```bash
npm run android:dev
```

## Documentation

`react-native-firebase` aims to match the Firebase Web API wherever possible. As a result, the tests are largely derived from the [Firebase Web API documentation](https://firebase.google.com/docs/reference/js/).


## Tests

Tests are bootstrapped and ran when the app is booted. The status of each test suite and individual test will update as and when a test has completed or errored.

### Running tests

Tests can be run by pressing the play button in the toolbar of the app. Test can be run individually, by suite, or all at once.

![Test suite Android](/tests/docs/assets/test-suite-screenshot-android.png?raw=true)


### Adding test

To add tests to an existing test suite, you need to pass a function to `addTests`.

#### Synchronous tests

Synchronous tests are created by passing a function to `it`. The next test is run immediately after the last line is executed.

```javascript
testSuite.addTests(({ describe, it }) => {
    describe('synchronous test', () => {

      it('does something correctly', () => {

      });

    });
});
```

#### Asynchronous tests

Tests can be asynchronous if they return a promise. The test suite waits for the promise to resolve before executing the next test.

```javascript
testSuite.addTests(({ describe, it }) => {
    describe('async successful test', () => {

      it('does something correctly', () => {
        return new Promise((resolve, reject) => {
          // ...
          resolve();
        });
      });

    });
});
```

Asynchronous tests can also be created using the `async` function syntax:

```javascript
testSuite.addTests(({ describe, it }) => {
    describe('async successful test', () => {

      it('does something correctly', async () => {
        // ...

        await somethingAsynchronous();
      });

    });
});
```

> When rejecting, always ensure a valid [JavaScript Error](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Error) is provided.

### Creating a new test suite

A test suite groups together test categories under the same Firebase feature. e.g. *Realtime Database* tests.

To add a new test suite:

1. Create a new directory within `src/tests`.
2. Create an `index.js` file.

In this file, you need to create an instance of `TestSuite` - see [TestSuite constructor](#testsuite-constructor).

```javascript
import TestSuite from 'lib/TestSuite';

const MyNewSuite = new TestSuite('Realtime Database Storage', 'Upload/Download storage tests');

export default MyNewSuite;
```

3. `addTests` is then used as many times as is necessary to add tests to the test suite, accepting a function that defines one or more tests.
4. The test suite must then be imported into `src/tests/index.js` and added to `testSuiteInstances` in order for it to be included in the list of test suites available to run in the app.

## TestSuite API

### TestSuite Constructor

The TestSuite constructor accepts 3 arguments:

- **name**: String containing the name of the test suite. e.g. 'Realtime Storage'
- **description**: String containing description of the test suite
- **firebase**: This is the object exported from `src/firebase` and contains both the native and web firebase instances.

```javascript
import firebase from '../firebase';

new TestSuite('Realtime Database Storage', 'firebase.database()', firebase);
```

### Test Definition

#### describe()

The `describe()` function takes 2 - 3 arguments:

- **description**: String describing the context or target of all the tests defined in `testDefinitions`
- **options**: (Optional) object of options:
    * **focus**: Boolean marking all the tests defined in `testDefinitions` (and any others marked as focused) as the only one(s) that should run
    * **pending**: Boolean marking all the tests defined in `testDefinitions` as excluded from running in the test suite
- **testDefinitions**: Function that defines 1 or more tests by calling `it`, `xit` or `fit`

```javascript
function testCategory({ describe }) {

  describe('a feature', () => {
    it('does something synchronously', () => {

    });

  });
}

export default testCategory;
```

`describe()` statements can be arbitrarily nested.

#### context()

`context()` is an alias for `describe()` provided as syntactical sugar. `xcontext()` and `fcontext()` work similar to `xdescribe()` and `fdescribe()`, respectively.

#### it()

The `it()` function takes 2 - 3 arguments:

- **description**: String describing the test defined in `testDefinition`
- **options**: (Optional) object of options:
    * **focus**: Boolean marking the test defined in `testDefinition` (and any others marked as focused) as the only one(s) that should run
    * **pending**: Boolean marking the test defined in `testDefinition` as excluded from running in the test suite
    * **timeout**: Time in milliseconds a test is allowed to execute before it's considered to have timed out. Default is 5000ms (5 seconds).
- **testDefinition**: Function that defines a test with one or more assertions. Can be a synchronous or asynchronous function. Functions that return a promise cause the test environment to wait for the promise to be resolved before proceding to the next test.

```javascript
it('does something synchronously', () => {

});

it('does something asynchronously', async () => {

});

it('does something else asynchronously', () => {
    return new Promise(/* ... */);
});
```

`it()` statements can *not* be nested.

#### xdescribe() & xit()

##### Pending Tests

You can mark all tests within a `describe` statement as pending by using the `xdescribe` function instead. The test will appear greyed out and will not be run as part of the test suite.

You can mark a single test as pending by using `xit` as you would `it`.

Tests should only be marked as pending temporarily, and should not normally be committed to the test suite unless they are fully implemented.

#### fdescribe() & fit()

##### Focused Tests

You can mark all tests within a `describe` statement as focused by using the `fdescribe` function instead. Tests that are focused will be the only ones that appear and run in the test suite until all tests are removed from being focused. This is useful for running and working on a few tests at a time.

You can mark a single test as focused by using `fit` as you would `it`.

#### Test Assertions

The assertion library Should.js is used in the tests. The complete list of available assertions is available in the [Should.js documentation](https://shouldjs.github.io).

#### Lifecycle methods

Four lifecycle methods are provided for each test context:

- **before** - Run once, before the current test context executes
- **beforeEach** - Run before every test in the current test context
- **after** - Run once, after the current test context has finished executing
- **afterEach** - Run after every test in the current test context

A new test context is created when the test suite encounters any of `describe`, `xdescribe`, `fdescribe`, `context`, `xcontext` or `fcontext`, and close again when it reaches the end of the block. Test contexts can be nested and lifecycle hooks set for parent contexts apply for all descendents.

Each lifecycle hook accepts either a synchronous function, a function that returns a promise or an `async` function.

```javascript
function testCategory({ before, beforeEach, afterEach, after }) {

  before(() => console.log('Before all tests start.'));
  beforeEach(() => console.log('Before every test starts.'));

  describe('sync successful test', function() {
    // ...
  });

  afterEach(() => console.log('After each test starts.'));
  after(() => console.log('After all tests are complete, with success or error.'));
}
```

An optional hash of options can also be passed as the first argument, defining one or more of the following values:

* **timeout**: Time in milliseconds a hook is allowed to execute before it's considered to have timed out. Default is 5000ms (5 seconds).

#### Accessing Firebase

`react-native-firebase` is available `firebase.native`:

```javascript
function testCategory({ describe, firebase }) {

  describe('sync successful test', 'category', function() {
    firebase.native.database();
  });
}
```

If you need to access the web API for Firebase to compare with the functionality of `react-native-firebase`, you can access it on `firebase.web`.

> All tests should be written in terms of `react-native-firebase`'s behaviour and should **not** include direct comparisons with the web API. It's available for reference, only.

## Development

### Running test suite against latest version of react-native-firebase

You can use the node module `wml` to automatically copy changes you make to `react-native-firebase` over to the test application so you can run the test suite against them.

1. Install `wml` globally:

```bash
npm install wml -g
```

2. Configure `wml` to copy changes from `react-native-firebase` to `react-native-firebase/tests/node_modules/react-native-firebase` is:

```bash
wml add /full/path/to/react-native-firebase /full/path/to/react-native-firebase/tests/node_modules/react-native-firebase
```

3. Start `wml`:

```bash
wml start
```

> JavaScript changes require restarting the React Native packager to take effect

> Java changes will need to be rebuilt in Android Studio

> Objective-C changes need to be rebuilt in Xcode

4. Stop `wml` when you are finished:
```bash
wml stop
```

### Debugging or viewing internals of the test suite

`react-native-firebase/tests` is compatible with [react-native-debugger](https://github.com/jhen0409/react-native-debugger) and is the recommended way to view the internal state of the test suite for development or troubleshooting.

It allows you to view state and prop values of the React component tree, view the actions and contents of the Redux store and view and interact with the debugging console.

Make sure **Remote JS Debugging** when running the application and close any chrome debugging windows that appear and start React Native Debugger.

### Running the internal tests

`react-native-firebase-tests` has its own tests to verify the testing framework is working as expected. These are run from the command line:

```bash
npm run internal-tests
```

## Troubleshooting

### Invalid React.podspec file: no implicit conversion of nil into String

This error occurs if you are using ruby version 2.1.2. Upgrade your version of ruby and try again.


### Unable to resolve module ../../../node_modules/react-native/packager/...

Run the packager separately, clearing the cache:

```bash
npm start -- --reset-cache
```
