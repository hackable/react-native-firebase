#ifndef RNFirebasePerformance_h
#define RNFirebasePerformance_h

#if __has_include(<React/RCTBridgeModule.h>)
#import <React/RCTBridgeModule.h>
#else // Compatibility for RN version < 0.40
#import "RCTBridgeModule.h"
#endif

@interface RNFirebasePerformance : NSObject <RCTBridgeModule> {

}

@end

#endif
